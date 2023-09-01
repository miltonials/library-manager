#ifndef OPCIONESOPERATIVAS_H
#define OPCIONESOPERATIVAS_H
/**
 * @file opcionesOperativas.h
 * @brief Funciones para gestionar las opciones operativas de la biblioteca (catalogo, prestamos, devoluciones, etc)
 * @version 1.0
 * @date 29/09/2023
 * @author @Andyporras,@miltonials
*/

/**
 * Mostrar libros.
 * 
 * Esta funcion muestra los libros de la biblioteca en pantalla.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
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
/**
 * Buscar libro por id.
 * 
 * Esta funcion busca un libro por id y retorna la direccion de memoria del libro.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @param id id del libro
 * @return direccion de memoria del libro
*/
Libro *buscarLibro_id(Biblioteca *dirM_biblioteca, int id) {
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  for (int i = 0; i < cantidadLibros; i++) {
    if (dirM_biblioteca->libros[i].id == id) {
      return &dirM_biblioteca->libros[i];
    }
  }
  return NULL;
}

/**
 * Existe libro.
 * 
 * Esta funcion busca un libro por titulo y retorna la direccion de memoria del libro.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @param titulo titulo del libro
 * @return direccion de memoria del libro
*/
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

/**
 * Formato archivo txt.
 * 
 * Esta funcion valida el formato de un archivo txt. Retorna 1 si el formato es correcto, 0 si el formato es incorrecto.
 * 
 * @param contenido contenido del archivo
 * @return 1 si el formato es correcto, 0 si el formato es incorrecto
*/
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

/**
 * Actualizar catalogo txt.
 * 
 * Esta funcion actualiza el catalogo de libros de la biblioteca con un archivo txt.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
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

/**
 * Opciones gestion catalogo.
 * 
 * Esta funcion muestra el menu de gestion de catalogo y captura la opcion.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
void opcionesGestionCatalogo(Biblioteca *dirM_biblioteca)
{
  int opcion = menuGestionCatalogo();

  while (opcion != 4) {
    switch (opcion) {
    case 1:
      actualizarCatalogo_txt(dirM_biblioteca);
      break;
    case 2:
      mostrarLibros(dirM_biblioteca);
      break;
    case 3:
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
/**
 * Historial prestamos.
 * 
 * Esta funcion muestra el historial de prestamos de la biblioteca en un rango de fechas indicado por el usuario.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
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
/**
 * Prestamos vencidos.
 * 
 * Esta funcion muestra los prestamos vencidos y proximos a vencer.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
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
/**
 * Listar prestamos.
 * 
 * Esta funcion lista los libros prestados. retorna una lista de estructuras EstadisticasLibro que contiene el id del libro, el nombre del libro y la cantidad de prestamos.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return lista de estructuras EstadisticasLibro
*/
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
/**
 * Ordenar libros mas prestados.
 * 
 * Esta funcion ordena una lista de estructuras EstadisticasLibro por cantidad de prestamos.
 * 
 * @param estadisticasLibros lista de estructuras EstadisticasLibro
 * @param cantidadLibros cantidad de libros
*/
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
/**
 * Top 3 producciones mas prestadas.
 * 
 * Esta funcion muestra los 3 libros mas prestados.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
void top3ProduccionesMasPrestadas(Biblioteca *dirM_biblioteca){
  EstadisticasLibro *estadisticasLibros = listarLibrosMasPrestados(dirM_biblioteca);
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  estadisticasLibros = ordenarLibrosMasPrestados(estadisticasLibros, cantidadLibros);
  int i;
  for (i = 0; i < 3; i++) {
    printf("Id: %d\n", estadisticasLibros[i].idLibro);
    printf("Titulo: %s\n", estadisticasLibros[i].nombreLibro);
    printf("Cantidad de prestamos: %d\n", estadisticasLibros[i].cantidadPrestamos);
    printf("\n");
  }
  // Liberar memoria asignada a las estructuras EstadisticasLibro
  for (int i = 0; i < cantidadLibros; i++) {
    free(estadisticasLibros[i].nombreLibro);
  }
}
/**
 * Listar usuarios mas prestamos.
 * 
 * Esta funcion lista los usuarios con mas prestamos. retorna una lista de estructuras EstadisticasUsuario que 
 * contiene la cedula del usuario, el nombre del usuario y la cantidad de prestamos.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return lista de estructuras EstadisticasUsuario
*/
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
/**
 * Ordenar usuarios mas prestamos.
 * 
 * Esta funcion ordena una lista de estructuras EstadisticasUsuario por cantidad de prestamos.
 * 
 * @param estadisticasUsuarios lista de estructuras EstadisticasUsuario
 * @param cantidadUsuarios cantidad de usuarios
 * @return lista de estructuras EstadisticasUsuario ordenada
*/
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
// B. Top 3 de usuarios con más préstamos.
/**
 * Top 3 usuarios mas prestamos.
 * 
 * Esta funcion muestra los 3 usuarios con mas prestamos.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
void top3UsuariosMasPrestamos(Biblioteca *dirM_biblioteca){
  EstadisticasUsuario *estadisticasUsuarios = listarUsuariosMasPrestamos(dirM_biblioteca);
  int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
  estadisticasUsuarios = ordenarUsuariosMasPrestamos(estadisticasUsuarios, cantidadUsuarios);
  int i;
  for (i = 0; i < 3; i++) {
    printf("Cedula: %s\n", estadisticasUsuarios[i].cedulaUsuario);
    printf("Nombre: %s\n", estadisticasUsuarios[i].nombreUsuario);
    printf("Cantidad de prestamos: %d\n", estadisticasUsuarios[i].cantidadPrestamos);
    printf("\n");
  }
  // Liberar memoria asignada a las estructuras EstadisticasUsuario
  for (int i = 0; i < cantidadUsuarios; i++) {
    free(estadisticasUsuarios[i].nombreUsuario);
  }
}
/**
 * Calcular monto recaudado.
 * 
 * Esta funcion calcula el monto recaudado de un prestamo.
 * 
 * @param fechaInicio fecha de inicio del prestamo
 * @param fechaFin fecha de fin del prestamo
 * @param fechaDevolucion fecha de devolucion del prestamo
 * @return monto recaudado
*/
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
/**
 * Obtener mes y año.
 * 
 * Esta funcion obtiene el mes y año de una fecha. retorna una cadena de caracteres con el mes y año. Ejemplo: 01/2021
 * 
 * @param fecha fecha
 * @return cadena de caracteres con el mes y año
*/
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
/**
 * Ordenar prestamos mes año mayor monto recaudado.
 * 
 * Esta funcion ordena una lista de estructuras MayorRecaudacionMes por monto recaudado.
 * 
 * @param mayorRecaudacionMes lista de estructuras MayorRecaudacionMes
 * @param cantidadPrestamos cantidad de prestamos
 * @return lista de estructuras MayorRecaudacionMes ordenada
*/
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

/**
 * Listar mes año mayor monto recaudado.
 * 
 * Esta funcion lista los meses y años con mayor monto recaudado. retorna una lista de estructuras MayorRecaudacionMes que contiene el 
 * id del prestamo, el mes y año y el monto recaudado.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @param cantidad cantidad de prestamos
 * @return lista de estructuras MayorRecaudacionMes
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
  MayorRecaudacionMes *mayorRecaudacionMesOrdenado = ordenarPrestamosMesAnoMayorMontoRecaudado(mayorRecaudacionMes, cantidadPrestamosFinalizados);
  *cantidad = cantidadPrestamosFinalizados;
  return mayorRecaudacionMesOrdenado;
}
/**
 * Top 5 meses mayor monto recaudado.
 * 
 * Esta funcion muestra los 5 meses con mayor monto recaudado.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
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
/**
 * Menu opciones estadisticas.
 * 
 * Esta funcion muestra el menu de opciones estadisticas y captura la opcion.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
void menuOpcionesEstadisticas(Biblioteca *dirM_biblioteca){
  int opcion = menuEstadisticas();
  while (opcion != 4) {
    switch (opcion) {
    case 1:
      top3ProduccionesMasPrestadas(dirM_biblioteca);
      pausar("Presione enter para volver al menú...");
      limpiarPantalla();
      break;
    case 2:
      top3UsuariosMasPrestamos(dirM_biblioteca);
      pausar("Presione enter para volver al menú...");
      limpiarPantalla();
      break;
    case 3:
      top5MesesMayorMontoRecaudado(dirM_biblioteca);
      pausar("Presione enter para volver al menú...");
      limpiarPantalla();
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
/**
 * Opciones operativas.
 * 
 * Esta funcion muestra el menu de opciones operativas y captura la opcion.
 * 
 * @param dirM_biblioteca direccion de memoria de la biblioteca
 * @return ninguna
*/
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
