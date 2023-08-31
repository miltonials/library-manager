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


Libro *buscarLibro_id(Biblioteca *dirM_biblioteca, int id) {
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  for (int i = 0; i < cantidadLibros; i++) {
    if (dirM_biblioteca->libros[i].id == id) {
      return &dirM_biblioteca->libros[i];
    }
  }
  return NULL;
}


Libro *existeLibro(Biblioteca *dirM_biblioteca, char *titulo)
{
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  int i = 0;
  for (i = 0; i < cantidadLibros; i++) {
    if (strcmp(dirM_biblioteca->libros[i].titulo, titulo) == 0) {
      return &dirM_biblioteca->libros[i];
    }
  }
  return NULL;
}

int formatoArchivo_txt(char *contenido)
{
  // char *contenido = leerArchivo(rutaArchivo);

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
  // Libro *libros = dirM_biblioteca->libros;

  char *linea = "";
  int librosAgregados = 0;

  int nLineaAct = 0;
  char *lineaCp = "";
  
  //formato del archivo: titulo|autor|anio|genero|resumen|cantidad  
  while ((linea = strsep(&contenido, "\n")) != NULL) {
    nLineaAct++;
    lineaCp = malloc(strlen(linea) + 1);
    strcpy(lineaCp, linea);

    if (formatoArchivo_txt(lineaCp) == 0) {
      printf("Linea %d con formato incorrecto.\n", nLineaAct);
      continue;
    }

    Libro libro = {0};
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
      libro.id = dirM_biblioteca->cantidadLibros + 1;
      dirM_biblioteca->libros[dirM_biblioteca->cantidadLibros] = libro;
      dirM_biblioteca->cantidadLibros++;
      librosAgregados++;

      actualizarCatalogo(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
      cargarLibros(dirM_biblioteca);

      printf("+ %s ha sido  agregado con el id %d\n", libro.titulo, libro.id);
    }
    else {
      printf("- %s\n", libro.titulo);
    }
  }

  printf("\n===============Se agregaron %d libros al catalogo.===============\n", librosAgregados);
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
            Usuario *usuario = existeUsuario(biblioteca, prestamos[i].cedulaUsuario);
            Libro *libro = buscarLibro_id(biblioteca, prestamos[i].idLibro);
            char *nombreUsuario = usuario->nombre;
            int diasTardios = tadiasEnDias(prestamos[i].fechaDevolucion, prestamos[i].fechaFin);
            printf("Identificador de prestamo: %d\n", prestamos[i].id);
            printf("Usuario: %s\n", nombreUsuario);
            // printf("Usuario: %s\n", prestamos[i].cedulaUsuario);
            if (prestamos[i].estado == 1){
              printf("Estado: activo\n");
            }else{
              printf("Estado: cancelado\n");
            }
            printf("Nombre: %s (%d)\n", libro->titulo, libro->id);
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
      Usuario *usuario = existeUsuario(biblioteca, prestamos[i].cedulaUsuario);
      Libro *libro = buscarLibro_id(biblioteca, prestamos[i].idLibro);
      dias = tadiasEnDias(obtenerFechaActual(), prestamos[i].fechaFin);
      // printf("fecha de devolucion: %s\n", prestamos[i].fechaFin);
      // printf("fecha actual: %s\n", obtenerFechaActual());
      // printf("dias: %d\n", dias);
      if(validarRangoFechas(prestamos[i].fechaFin, obtenerFechaActual()) == 1){
        if (dias >= 1 ) {
          
          printf("Identificador de prestamo: %d\n", prestamos[i].id);
          printf("Usuario: %s\n", usuario->nombre);
          printf("Fecha de entrega: %s\n", prestamos[i].fechaFin);
          printf("Nombre: %s\n", libro->titulo);
          printf("Dias tardios: %d\n", dias);
          printf("\n");
        }
      }else{
        // printf("prestamo a vencer\n");
        dias = diferenciaDias(obtenerFechaActual(), prestamos[i].fechaFin);
        // printf("dias: %d\n", dias);
        if (dias >= 0 && dias <= 3) {
          printf("Prestamo proximo a vencer\n");
          printf("Identificador de prestamo: %d\n", prestamos[i].id);
          printf("Usuario: %s\n", usuario->nombre);
          printf("Fecha de entrega: %s\n", prestamos[i].fechaFin);
          printf("Nombre: %s\n", libro->titulo);
          printf("Dias para vencer: %d\n", dias);
          printf("\n");
        }
      }
    }
  }
}
EstadisticasLibro *listarLibrosMasPrestados(Biblioteca *dirM_biblioteca){
  Libro *libros = dirM_biblioteca->libros;
  Prestamo *prestamos = dirM_biblioteca->prestamos;
  int cantidadPrestamos = dirM_biblioteca->cantidadPrestamos;
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  EstadisticasLibro *estadisticasLibros = malloc(sizeof(EstadisticasLibro) * cantidadLibros);
  int i;
  int j;
  int cantidadPrestamosLibro = 0;
  for (i = 0; i < cantidadLibros; i++) {
    cantidadPrestamosLibro = 0;
    for (j = 0; j < cantidadPrestamos; j++) {
      if (libros[i].id == prestamos[j].idLibro) {
        cantidadPrestamosLibro++;
      }
    }
    estadisticasLibros[i].idLibro = libros[i].id;
    estadisticasLibros[i].nombreLibro = libros[i].titulo;
    estadisticasLibros[i].cantidadPrestamos = cantidadPrestamosLibro;
  }
  return estadisticasLibros;
}
EstadisticasLibro *ordenarLibrosMasPrestados(EstadisticasLibro *estadisticasLibros, int cantidadLibros){
  int i;
  int j;
  EstadisticasLibro aux;
  for (i = 0; i < cantidadLibros; i++) {
    for (j = 0; j < cantidadLibros; j++) {
      if (estadisticasLibros[i].cantidadPrestamos > estadisticasLibros[j].cantidadPrestamos) {
        aux = estadisticasLibros[i];
        estadisticasLibros[i] = estadisticasLibros[j];
        estadisticasLibros[j] = aux;
      }
    }
  }
  return estadisticasLibros;
}
// A. Top 3 de producciones (nombre) más prestadas. 
void top3ProduccionesMasPrestadas(Biblioteca *dirM_biblioteca){
  EstadisticasLibro *estadisticasLibros = listarLibrosMasPrestados(dirM_biblioteca);
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  estadisticasLibros = ordenarLibrosMasPrestados(estadisticasLibros, cantidadLibros);
  int i;
  for (i = 0; i < 3; i++) {
    printf("Titulo: %s\n", estadisticasLibros[i].nombreLibro);
    printf("Cantidad de prestamos: %d\n", estadisticasLibros[i].cantidadPrestamos);
    printf("\n");
  }
}
EstadisticasUsuario *listarUsuariosMasPrestamos(Biblioteca *dirM_biblioteca){
  Usuario *usuarios = dirM_biblioteca->usuarios;
  Prestamo *prestamos = dirM_biblioteca->prestamos;
  int cantidadPrestamos = dirM_biblioteca->cantidadPrestamos;
  int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
  EstadisticasUsuario *estadisticasUsuarios = malloc(sizeof(EstadisticasUsuario) * cantidadUsuarios);
  int i;
  int j;
  int cantidadPrestamosUsuario = 0;
  for (i = 0; i < cantidadUsuarios; i++) {
    cantidadPrestamosUsuario = 0;
    for (j = 0; j < cantidadPrestamos; j++) {
      if (strcmp(usuarios[i].cedula, prestamos[j].cedulaUsuario) == 0) {
        cantidadPrestamosUsuario++;
      }
    }
    estadisticasUsuarios[i].cedulaUsuario = usuarios[i].cedula;
    estadisticasUsuarios[i].nombreUsuario = usuarios[i].nombre;
    estadisticasUsuarios[i].cantidadPrestamos = cantidadPrestamosUsuario;
  }
  return estadisticasUsuarios;
} 
EstadisticasUsuario *ordenarUsuariosMasPrestamos(EstadisticasUsuario *estadisticasUsuarios, int cantidadUsuarios){
  int i;
  int j;
  EstadisticasUsuario aux;
  for (i = 0; i < cantidadUsuarios; i++) {
    for (j = 0; j < cantidadUsuarios; j++) {
      if (estadisticasUsuarios[i].cantidadPrestamos > estadisticasUsuarios[j].cantidadPrestamos) {
        aux = estadisticasUsuarios[i];
        estadisticasUsuarios[i] = estadisticasUsuarios[j];
        estadisticasUsuarios[j] = aux;
      }
    }
  }
  return estadisticasUsuarios;
}
void top3UsuariosMasPrestamos(Biblioteca *dirM_biblioteca){
  EstadisticasUsuario *estadisticasUsuarios = listarUsuariosMasPrestamos(dirM_biblioteca);
  int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
  estadisticasUsuarios = ordenarUsuariosMasPrestamos(estadisticasUsuarios, cantidadUsuarios);
  int i;
  for (i = 0; i < 3; i++) {
    printf("Nombre: %s\n", estadisticasUsuarios[i].nombreUsuario);
    printf("Cantidad de prestamos: %d\n", estadisticasUsuarios[i].cantidadPrestamos);
    printf("\n");
  }
}
int calcularMontoRecaudado(const char *fechaInicio,const char *fechaFin,const char *fechaDevolucion){
  int dias = diferenciaDias(fechaInicio, fechaDevolucion);
  int diasTardios = tadiasEnDias(fechaDevolucion, fechaFin);
  int tarificacion;
  int tarificacionTardia;
  int monto;
  // printf("dias: %d\n", dias);
  // printf("dias tardios: %d\n", diasTardios);
  if(dias <= 7){
    tarificacion = 150;
    tarificacionTardia = 75;
  }
  else if(dias <= 15){
    tarificacion = 125;
    tarificacionTardia = 50;
  }
  else{
    tarificacion = 100;
    tarificacionTardia = 25;
  }
  monto = (dias * tarificacion) + (diasTardios * tarificacionTardia);
  return monto;
}

char *obtenerMesAnio(const char *fecha) {
  if (strlen(fecha) != 10) {
    return NULL; // Manejo de errores: la fecha no tiene el formato esperado
  }

  char *mesAnio = malloc(8);
  if (mesAnio == NULL) {
    return NULL; // Manejo de errores: no se pudo asignar memoria
  }

  char mes[3];
  char anio[5];

  strncpy(mes, fecha + 3, 2); // Avanza 3 caracteres para saltar el día y el "/"
  mes[2] = '\0';

  strncpy(anio, fecha + 6, 4);
  anio[4] = '\0';

  snprintf(mesAnio, 8, "%s/%s", mes, anio);


  return mesAnio;
}

MayorRecaudacionMes *ordenarPrestamosMesAnoMayorMontoRecaudado(MayorRecaudacionMes *mayorRecaudacionMes, int cantidadPrestamos) {
  int i, j;
  MayorRecaudacionMes aux;

  for (i = 0; i < cantidadPrestamos - 1; i++) {
    for (j = i + 1; j < cantidadPrestamos; j++) {
      // Comparar por montos recaudados en orden descendente
      if (mayorRecaudacionMes[i].montosRecaudados < mayorRecaudacionMes[j].montosRecaudados) {
        aux = mayorRecaudacionMes[i];
        mayorRecaudacionMes[i] = mayorRecaudacionMes[j];
        mayorRecaudacionMes[j] = aux;
      }
    }
  }

  return mayorRecaudacionMes;
}

/*
retorna una lista de MayorRecaudacionMes
*/
MayorRecaudacionMes *listarMesAnoMayorMontoRecaudado(Biblioteca *dirM_biblioteca, int *cantidad) {
  Prestamo *prestamos = dirM_biblioteca->prestamos;
  int cantidadPrestamos = dirM_biblioteca->cantidadPrestamos;

  MayorRecaudacionMes *mayorRecaudacionMes = malloc(sizeof(MayorRecaudacionMes) * cantidadPrestamos);
  if (mayorRecaudacionMes == NULL) {
      return NULL; // Manejo de errores: no se pudo asignar memoria
  }

  int cantidadPrestamosFinalizados = 0;

  for (int i = 0; i < cantidadPrestamos; i++) {
      if (prestamos[i].estado == 0) {
          char *mesAnio = obtenerMesAnio(prestamos[i].fechaInicio);

          int existe = 0;
          for (int j = 0; j < cantidadPrestamosFinalizados; j++) {
              if (strcmp(mesAnio, mayorRecaudacionMes[j].mesAnio) == 0) {
                  existe = 1;
                  break;
              }
          }

          if (existe == 0) {
              // printf("No existe\n");
              mayorRecaudacionMes[cantidadPrestamosFinalizados].idPrestamo = prestamos[i].id;
              mayorRecaudacionMes[cantidadPrestamosFinalizados].mesAnio = strdup(mesAnio);
              mayorRecaudacionMes[cantidadPrestamosFinalizados].montosRecaudados = calcularMontoRecaudado(prestamos[i].fechaInicio, prestamos[i].fechaFin, prestamos[i].fechaDevolucion);

              cantidadPrestamosFinalizados++;
          } else {
              // Buscar la posición del mes y año en la lista
              int posicion = -1;
              for (int j = 0; j < cantidadPrestamosFinalizados; j++) {
                  if (strcmp(mesAnio, mayorRecaudacionMes[j].mesAnio) == 0) {
                      posicion = j;
                      break;
                  }
              }

              if (posicion != -1) {
                  mayorRecaudacionMes[posicion].montosRecaudados += calcularMontoRecaudado(prestamos[i].fechaInicio, prestamos[i].fechaFin, prestamos[i].fechaDevolucion);
              }
          }

          free(mesAnio);
      }
  }
  //imprimir lista
  /*
  for(int i = 0; i < cantidadPrestamosFinalizados; i++){
    printf("Mes anio: %s\n", mayorRecaudacionMes[i].mesAnio);
    printf("Monto recaudado: %d\n", mayorRecaudacionMes[i].montosRecaudados);
    printf("\n");
  }
  */
  MayorRecaudacionMes *mayorRecaudacionMesOrdenado = ordenarPrestamosMesAnoMayorMontoRecaudado(mayorRecaudacionMes, cantidadPrestamosFinalizados);
  *cantidad = cantidadPrestamosFinalizados;
  return mayorRecaudacionMesOrdenado;
}

void top5MesesMayorMontoRecaudado(Biblioteca *dirM_biblioteca){
  int cantidad;
  MayorRecaudacionMes *mayorRecaudacionMes = listarMesAnoMayorMontoRecaudado(dirM_biblioteca, &cantidad);
  
  int cantidadMayorRecaudacionMes = 0;
  while (mayorRecaudacionMes[cantidadMayorRecaudacionMes].mesAnio != NULL && cantidadMayorRecaudacionMes < 5 ) {
    cantidadMayorRecaudacionMes++;
  }

  for (int i = 0; i < cantidadMayorRecaudacionMes; i++) {
    if(i == cantidad){
      break;
    }
    printf("Mes anio: %s\n", mayorRecaudacionMes[i].mesAnio);
    printf("Monto recaudado: %d\n", mayorRecaudacionMes[i].montosRecaudados);
    printf("\n");

  }
  
  // Liberar memoria asignada a las estructuras MayorRecaudacionMes
  for (int i = 0; i < cantidadMayorRecaudacionMes; i++) {
    if(i == cantidad){
      break;
    }
    free(mayorRecaudacionMes[i].mesAnio);
  }
  free(mayorRecaudacionMes);
}

/*
Estadísticas Se deberán mostrar las siguientes estadísticas (indican el código o letra de esta): 
A. Top 3 de producciones (nombre) más prestadas. 
B. Top 3 de usuarios con más préstamos.
C. Top 5 de mes-año con mayor monto recaudado (según fecha de inicio).
*/
void menuOpcionesEstadisticas(Biblioteca *dirM_biblioteca){
  int opcion = menuEstadisticas();
  while (opcion != 4) {
    switch (opcion) {
    case 1:
      top3ProduccionesMasPrestadas(dirM_biblioteca);
      break;
    case 2:
      top3UsuariosMasPrestamos(dirM_biblioteca);
      break;
    case 3:
      printf("Top 5 de mes-año con mayor monto recaudado (según fecha de inicio).\n");
      top5MesesMayorMontoRecaudado(dirM_biblioteca);
      break;
    case 4:
      printf("Volver.\n");
      break;
    default:
      printf("Opcion invalida.\n");
      break;
    }
    opcion = menuEstadisticas();
  }

}

void opcionesOperativas(Biblioteca *dirM_biblioteca){
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
      menuOpcionesEstadisticas(dirM_biblioteca);
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
