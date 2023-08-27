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
      printf("Historial de prestamos.\n");
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
