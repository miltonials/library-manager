#ifndef OPCIONESOPERATIVAS_H
#define OPCIONESOPERATIVAS_H

void mostrarLibros(Biblioteca *dirM_biblioteca)
{
  printf("********************\n");
  printf("*Catálogo de libros*\n");
  printf("********************\n");

  for (int i = 0; i < dirM_biblioteca->cantidadLibros; i++) {
    printf("Título: %s\n", dirM_biblioteca->libros[i].titulo);
    printf("Autor: %s\n", dirM_biblioteca->libros[i].autor);
    printf("Año: %d\n", dirM_biblioteca->libros[i].anio);
    printf("Genero: %s\n", dirM_biblioteca->libros[i].genero);
    printf("Resumen: %s\n", dirM_biblioteca->libros[i].resumen);
    printf("Cantidad: %d\n", dirM_biblioteca->libros[i].cantidad);
    printf("\n");
  }

  pausar("Presione enter para volver al menú...");
  limpiarPantalla();
}

Libro *existeLibro(Biblioteca *dirM_biblioteca, char *titulo)
{
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  for (int i = 0; i < cantidadLibros; i++) {
    if (strcmp(dirM_biblioteca->libros[i].titulo, titulo) == 0) {
      return &dirM_biblioteca->libros[i];
    }
  }
  return NULL;
}

int formatoArchivo_txt(char *rutaArchivo)
{
  char *contenido = leerArchivo(rutaArchivo);

  char *linea = "";
  int campos = 0;
  while ((linea = strsep(&contenido, "\n")) != NULL) {
    char *campo = "";
    while ((campo = strsep(&linea, "|")) != NULL) {
      campos++;
    }
    if (campos != 6) {
      return 0;
    }
    campos = 0;
  }
  return 1;
}

void actualizarCatalogo_txt(Biblioteca *dirM_biblioteca)
{
  char rutaArchivo[200];
  printf("Ingrese la ruta del archivo: ");
  scanf("%199s", rutaArchivo);

  char *contenido = leerArchivo(rutaArchivo);
  Libro *libros = dirM_biblioteca->libros;
  char *linea = "";
  int librosAgregados = 0;

  if (formatoArchivo_txt(rutaArchivo) == 0) {
    printf("El archivo no tiene el formato correcto.\n");
    return;
  }

  //formato del archivo: titulo|autor|anio|genero|resumen|cantidad  
  while ((linea = strsep(&contenido, "\n")) != NULL) {
    Libro libro;
    char *campo = "";
    int campoActual = 0;
    while ((campo = strsep(&linea, "|")) != NULL) {
      switch (campoActual) {
      case 0:
        libro.titulo = malloc(strlen(campo) + 1);
        strcpy(libro.titulo, campo);
        break;
      case 1:
        libro.autor = malloc(strlen(campo) + 1);
        strcpy(libro.autor, campo);
        break;
      case 2:
        libro.anio = atoi(campo);
        break;
      case 3:
        libro.genero = malloc(strlen(campo) + 1);
        strcpy(libro.genero, campo);
        break;
      case 4:
        libro.resumen = malloc(strlen(campo) + 1);
        strcpy(libro.resumen, campo);
        break;
      case 5:
        libro.cantidad = atoi(campo);
        break;
      default:
        break;
      }
      campoActual++;
    }
    if (existeLibro(dirM_biblioteca, libro.titulo) == NULL) {
      //mostrar libro en un sola linea solo el titulo
      printf("+ %s\n", libro.titulo);
      libros[dirM_biblioteca->cantidadLibros] = libro;
      dirM_biblioteca->cantidadLibros++;
      librosAgregados++;
    }
    else {
      printf("- %s\n", libro.titulo);
    }
  }

  if (librosAgregados > 0) {
    actualizarBiblioteca(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
    printf("\n===============Se agregaron %d libros al catalogo.===============\n", librosAgregados);
  }
  else {
    printf("No se agregaron libros al catalogo.\n");
  }

  pausar("Presione enter para volver al menú...");
  limpiarPantalla();
}

void opcionesGestionCatalogo(Biblioteca *dirM_biblioteca)
{
  int opcion = menuGestionCatalogo();

  while (opcion != 5) {
    switch (opcion) {
    case 1:
      actualizarCatalogo_txt(dirM_biblioteca);
      break;
    case 2:
      printf("Modificar libro.\n");
      break;
    case 3:
      printf("Eliminar libro.\n");
      break;
    case 4:
      mostrarLibros(dirM_biblioteca);
      break;
    case 5:
      printf("Volver.\n");
      break;
    default:
      printf("Opcion invalida.\n");
      break;
    }
    opcion = menuGestionCatalogo();
  }
}

//El sistema deberá mostrar todos los préstamos generados en el sistema en un rango de fechas indicado por el usuario (inicio-fin)
//-considerando la fecha de entrega-, la información a mostrar será: identificador de préstamo, usuario, estado
//(activo o finalizado), nombre, identificador de ejemplar y si presentó entrega tardía (si o no) -aquellos con estado finalizado-.
void historialPrestamos(Biblioteca *biblioteca) {
  char fechaInicio[20];
  char fechaFinal[20];
  printf("Ingrese la fecha de inicio: ");
  scanf("%s", fechaInicio);
  printf("Ingrese la fecha final: ");
  scanf("%s", fechaFinal);

  if (formatoFecha(fechaInicio) == 1 || formatoFecha(fechaFinal) == 1) {
    //validar que la fecha de inicio sea menor a la fecha final
    if (validarRangoFechas(fechaInicio, fechaFinal) == 1) {
      Prestamo *prestamos = biblioteca->prestamos;
      int cantidadPrestamos = biblioteca->cantidadPrestamos;
      int i;
      for (i = 0; i < cantidadPrestamos; i++) {
        if(prestamos[i].fechaDevolucion == NULL) {
          continue;
        }else{
          if ( validarRangoFechas(fechaInicio, prestamos[i].fechaDevolucion) == 1 && validarRangoFechas(prestamos[i].fechaDevolucion, fechaFinal) == 1) {
            char *nombreUsuario = buscarNombre(biblioteca, prestamos[i].cedulaUsuario);
            int diasTardios = tadiasEnDias(prestamos[i].fechaDevolucion, prestamos[i].fechaFin);
            printf("Identificador de prestamo: %d\n", prestamos[i].id);
            printf("Usuario: %s\n", nombreUsuario);
            // printf("Usuario: %s\n", prestamos[i].cedulaUsuario);
            if (prestamos[i].estado == 1){
              printf("Estado: activo\n");
            }else{
              printf("Estado: cancelado\n");
            }
            printf("Nombre: %s\n", prestamos[i].tituloLibro);
            printf("Dias tardios: %d\n", diasTardios);
            printf("\n");
          }
        }
      }
    }else {
      printf("La fecha de inicio no puede ser mayor a la fecha final\n");
    }
  }
  else {
    printf("El formato de fecha es incorrecto\n");
  }
}
/*
El sistema deberá mostrar los préstamos vencidos y próximos a vencer (préstamos que vencen de 0 a 3 días),
 la información a mostrar por cada préstamo será: identificador de préstamo, usuario,
fecha de entrega, nombre e identificador de ejemplar. Considera fecha de sistema.*/
void prestamosVencidos(Biblioteca *biblioteca) {
  Prestamo *prestamos = biblioteca->prestamos;
  int cantidadPrestamos = biblioteca->cantidadPrestamos;
  int i;
  int dias;
  for (i = 0; i < cantidadPrestamos; i++) {
    // printf("fecha de devolucion: %s\n", obtenerFechaActual());
    if(prestamos[i].fechaDevolucion == NULL) {
      continue;
    }else{
      dias = tadiasEnDias(obtenerFechaActual(), prestamos[i].fechaFin);
      // printf("fecha de devolucion: %s\n", prestamos[i].fechaFin);
      // printf("fecha actual: %s\n", obtenerFechaActual());
      // printf("dias: %d\n", dias);
      if(validarRangoFechas(prestamos[i].fechaFin, obtenerFechaActual()) == 1){
        if (dias >= 1 ) {
          char *nombreUsuario = buscarNombre(biblioteca, prestamos[i].cedulaUsuario);
          printf("Identificador de prestamo: %d\n", prestamos[i].id);
          printf("Usuario: %s\n", nombreUsuario);
          printf("Fecha de entrega: %s\n", prestamos[i].fechaFin);
          printf("Nombre: %s\n", prestamos[i].tituloLibro);
          printf("Dias tardios: %d\n", dias);
          printf("\n");
        }
      }else{
        // printf("prestamo a vencer\n");
        dias = diferenciaDias(obtenerFechaActual(), prestamos[i].fechaFin);
        // printf("dias: %d\n", dias);
        if (dias >= 0 && dias <= 3) {
          char *nombreUsuario = buscarNombre(biblioteca, prestamos[i].cedulaUsuario);
          printf("Prestamo proximo a vencer\n");
          printf("Identificador de prestamo: %d\n", prestamos[i].id);
          printf("Usuario: %s\n", nombreUsuario);
          printf("Fecha de entrega: %s\n", prestamos[i].fechaFin);
          printf("Nombre: %s\n", prestamos[i].tituloLibro);
          printf("Dias para vencer: %d\n", dias);
          printf("\n");
        }
      }
    }
  }
}

void opcionesOperativas(Biblioteca *dirM_biblioteca)
{
  int opcion = menuOpcionesOperativas();
  while (opcion != 6) {
    switch (opcion) {
    case 1:
      opcionesGestionCatalogo(dirM_biblioteca);
      break;
    case 2:
      opcionGestionUsuarios(dirM_biblioteca);
      break;
    case 3:
      historialPrestamos(dirM_biblioteca);
      break;
    case 4:
      prestamosVencidos(dirM_biblioteca);
      break;
    case 5:
      printf("Estadisticas.\n");
      break;
    case 6:
      printf("Volver.\n");
      break;
    default:
      printf("Opcion invalida.\n");
      break;
    }
    opcion = menuOpcionesOperativas();
  }
}

#endif // OPCIONESOPERATIVAS_H
