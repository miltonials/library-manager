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
  int diaInicio, mesInicio, anioInicio;
  int diaFin, mesFin, anioFin;
  printf("Ingrese la fecha de inicio (dd/mm/aaaa): ");
  scanf("%d/%d/%d", &diaInicio, &mesInicio, &anioInicio);
  printf("Ingrese la fecha de fin (dd/mm/aaaa): ");
  scanf("%d/%d/%d", &diaFin, &mesFin, &anioFin);

  time_t tiempoInicio = {0};
  struct tm *tiempoInicioInfo = localtime(&tiempoInicio);
  tiempoInicioInfo->tm_mday = diaInicio;
  tiempoInicioInfo->tm_mon = mesInicio - 1;
  tiempoInicioInfo->tm_year = anioInicio - 1900;
  tiempoInicioInfo->tm_hour = 0;
  tiempoInicioInfo->tm_min = 0;
  tiempoInicioInfo->tm_sec = 0;
  tiempoInicio = mktime(tiempoInicioInfo);

  time_t tiempoFin = {0};
  struct tm *tiempoFinInfo = localtime(&tiempoFin);
  tiempoFinInfo->tm_mday = diaFin;
  tiempoFinInfo->tm_mon = mesFin - 1;
  tiempoFinInfo->tm_year = anioFin - 1900;
  tiempoFinInfo->tm_hour = 0;
  tiempoFinInfo->tm_min = 0;
  tiempoFinInfo->tm_sec = 0;
  tiempoFin = mktime(tiempoFinInfo);

  int cantidadPrestamos = biblioteca->cantidadPrestamos;

  
  // for (int i = 0; i < cantidadPrestamos; i++) {
  //   time_t tiempoPrestamo = biblioteca->prestamos[i].fechaPrestamo;
  //   time_t tiempoEntrega = biblioteca->prestamos[i].fechaEntrega;
  //   if (tiempoPrestamo >= tiempoInicio && tiempoPrestamo <= tiempoFin) {
  //     printf("Identificador de prestamo: %d\n", biblioteca->prestamos[i].idPrestamo);
  //     printf("Usuario: %s\n", biblioteca->prestamos[i].usuario);
  //     printf("Estado: %s\n", biblioteca->prestamos[i].estado);
  //     printf("Nombre: %s\n", biblioteca->prestamos[i].nombre);
  //     printf("Identificador de ejemplar: %s\n", biblioteca->prestamos[i].idEjemplar);
  //     if (strcmp(biblioteca->prestamos[i].estado, "finalizado") == 0) {
  //       if (tiempoEntrega > tiempoPrestamo) {
  //         printf("Entrega tardia: si\n");
  //       }
  //       else {
  //         printf("Entrega tardia: no\n");
  //       }
  //     }
  //     printf("\n");
  //   }
  // }


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
      printf("Vencimiento de prestamos.\n");
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
