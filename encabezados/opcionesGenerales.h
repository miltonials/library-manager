#ifndef OPCIONESGENERALES_H
#define OPCIONESGENERALES_H

void prestamoApdf(Biblioteca *dirM_biblioteca, Prestamo *prestamo, Usuario *usuario, Libro *libro) {
    char contenido[500];  // Ajusta el tamaño según tus necesidades
    //la ruta es dirM_biblioteca->rutaArchivos más la palabra comprobantes
    char *ruta = malloc(strlen(dirM_biblioteca->rutaArchivos) + strlen("comprobantes") + 1);
    strcpy(ruta, dirM_biblioteca->rutaArchivos);
    strcat(ruta, "comprobantes/");


    // Crear el contenido del PDF utilizando los datos del préstamo, usuario y libro
    snprintf(contenido, sizeof(contenido),
             "Detalles del Préstamo:\n\n"
             "ID de Préstamo: %d\n"
             "Cédula de Usuario: %s (%s)\n"
             "Título de Libro: %s\n"
             "Fecha de Inicio: %s\n"
             "Fecha de Fin: %s\n",
             prestamo->id, prestamo->cedulaUsuario, usuario->nombre,
             libro->titulo, prestamo->fechaInicio, prestamo->fechaFin);

    // Generar el nombre del archivo (puedes personalizarlo)
    char nombreArchivo[50];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "prestamo_%d", prestamo->id);

    // Llamar a la función para crear el PDF
    printf(" %s", contenido);
    crearPDF(ruta, nombreArchivo, contenido);

    printf("PDF de préstamo creado: %s\n", nombreArchivo);
}


void generarComprobante(Biblioteca *dirM_biblioteca, Usuario *usuario, Libro* libro, char *fechaInicio, char *fechaFinal){
  //validar que este disponible el libro
  Prestamo *prestamos = dirM_biblioteca->prestamos;
  Libro *libros = dirM_biblioteca->libros;
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  int cantidadPrestamos = dirM_biblioteca->cantidadPrestamos;
  int libroDisponible = libro->cantidad;
  int cedula = atoi(usuario->cedula);
  char *titulo;
  titulo = malloc(strlen(libro->titulo) + 1);
  strcpy(titulo, libro->titulo);

  if(libroDisponible > 0){
    //generar comprobante
    Prestamo prestamo;
    prestamo.id = cantidadPrestamos + 1;
    prestamo.tituloLibro = malloc(strlen(titulo) + 1);
    strcpy(prestamo.tituloLibro, titulo);
    prestamo.cedulaUsuario = malloc(strlen(usuario->cedula) + 1);
    strcpy(prestamo.cedulaUsuario, usuario->cedula);
    prestamo.fechaInicio = malloc(strlen(fechaInicio) + 1);
    strcpy(prestamo.fechaInicio, fechaInicio);
    prestamo.fechaFin = malloc(strlen(fechaFinal) + 1);
    strcpy(prestamo.fechaFin, fechaFinal);
    prestamo.estado = 1;

    prestamos = realloc(prestamos, sizeof(Prestamo) * (cantidadPrestamos + 1));
    prestamos[cantidadPrestamos] = prestamo;
    dirM_biblioteca->prestamos = prestamos;
    dirM_biblioteca->cantidadPrestamos++;

    libro->cantidad--;

    actualizarPrestamos(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
    actualizarCatalogo(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
    prestamoApdf(dirM_biblioteca, &prestamo, usuario, libro);
    printf("El libro %s ha sido prestado al usuario %s\n", titulo, usuario->nombre);
  }else{
    printf("El libro no esta disponible\n");
  }
}

int formatoFecha(char *fecha) {
  //fecha con formato: dd/mm/aaaa -> 10 caracteres
  
  if (strlen(fecha) != 10) {
    return 0;
  }

  char *dia = malloc(3);
  char *mes = malloc(3);
  char *anio = malloc(5);

  dia[0] = fecha[0];
  dia[1] = fecha[1];
  dia[2] = '\0';

  mes[0] = fecha[3];
  mes[1] = fecha[4];
  mes[2] = '\0';

  anio[0] = fecha[6];
  anio[1] = fecha[7];
  anio[2] = fecha[8];
  anio[3] = fecha[9];
  anio[4] = '\0';

  int diaInt = atoi(dia);
  int mesInt = atoi(mes);
  int anioInt = atoi(anio);

  if (diaInt < 1 || diaInt > 31) {
    return 0;
  }

  if (mesInt < 1 || mesInt > 12) {
    return 0;
  }

  if (anioInt < 2021) {
    return 0;
  }

  return 1;
}

void prestamoEjemplar(Biblioteca *dirM_biblioteca){
  Usuario *usuarios = dirM_biblioteca->usuarios;
  Libro *libros = dirM_biblioteca->libros;
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
  char cedula[20];
  char titulo[50];
  char fechaInicio[20];
  char fechaFinal[20];
  
  printf("Ingrese la cedula del usuario: ");
  scanf(" %[^\n]s", cedula);
  // se pide el titulo del libro se acepta espacios
  printf("Ingrese el titulo del libro: ");
  scanf(" %[^\n]s", titulo);
  printf("Ingrese la fecha de inicio: ");
  scanf("%s", fechaInicio);
  printf("Ingrese la fecha final: ");
  scanf("%s", fechaFinal);

  Libro *libro = existeLibro(dirM_biblioteca, titulo);
  Usuario *usuario = existeUsuario(dirM_biblioteca, cedula);

  if (usuario != NULL) {
    if (libro != NULL) {
      if(formatoFecha(fechaInicio) == 1 || formatoFecha(fechaFinal) == 1){
        generarComprobante(dirM_biblioteca, usuario, libro, fechaInicio, fechaFinal);
      }
      else{
        printf("El formato de fecha es incorrecto\n");
      }
    }
    else {
      printf("El libro no existe\n");
    }
  }
  else {
    printf("El usuario no existe\n");
  }

  pausar("Presione enter para volver al menú...");
  limpiarPantalla();
}

void opcionesGenerales(Biblioteca *dirM_biblioteca) {
  int opcion = menuOpcionesGenerales();
  while(opcion != 5) {
    switch (opcion){
    case 1:
      printf("Busqueda simple.\n");
      break;
    case 2:
      printf("Busqueda avanzada.\n");
      break;
    case 3:
      printf("Prestamo de ejemplar.\n");
      prestamoEjemplar(dirM_biblioteca);
      break;
    case 4:
      printf("Devolucion de ejemplar.\n");
      break;
    case 5:
      printf("Volver.\n");
      break;
    default:
      printf("Opcion invalida.\n");
      break;
    }
    opcion = menuOpcionesGenerales();
  }
}


#endif // OPCIONESGENERALES_H