#ifndef OPCIONESOPERATIVAS_H
#define OPCIONESOPERATIVAS_H

void imprimirLibros(Libro *libros, int cantidadLibros)
{
  printf("********************\n");
  printf("*Catálogo de libros*\n");
  printf("********************\n");

  for (int i = 0; i < cantidadLibros - 1; i++) {
    printf("Título: %s\n", libros[i].titulo);
    printf("Autor: %s\n", libros[i].autor);
    printf("Año: %d\n", libros[i].anio);
    printf("Genero: %s\n", libros[i].genero);
    printf("Resumen: %s\n", libros[i].resumen);
    printf("Cantidad: %d\n", libros[i].cantidad);
    printf("\n");
  }
}

Libro *cargarLibros(char *rutaArchivo)
{
  char *contenido = leerArchivo(rutaArchivo);
  if (contenido == NULL) {
    return NULL;
  }

  struct json_object *parsed_json = json_tokener_parse(contenido);
  free(contenido);

  if (parsed_json == NULL) {
    printf("Error al parsear el archivo\n");
    return NULL;
  }

  size_t n = json_object_array_length(parsed_json);
  Libro *libros = malloc(sizeof(Libro) * n);

  for (size_t i = 0; i < n; i++) {
    struct json_object *obj = json_object_array_get_idx(parsed_json, i);

    struct json_object *titulo_obj, *autor_obj, *anio_obj, *genero_obj, *resumen_obj, *cantidad_obj;

    // json_object_object_get_ex(obj, "id", &id_obj);
    json_object_object_get_ex(obj, "titulo", &titulo_obj);
    json_object_object_get_ex(obj, "autor", &autor_obj);
    json_object_object_get_ex(obj, "anio", &anio_obj);
    json_object_object_get_ex(obj, "genero", &genero_obj);
    json_object_object_get_ex(obj, "resumen", &resumen_obj);
    json_object_object_get_ex(obj, "cantidad", &cantidad_obj);

    // int id = json_object_get_int(id_obj);
    const char *titulo = json_object_get_string(titulo_obj);
    const char *autor = json_object_get_string(autor_obj);
    int anio = json_object_get_int(anio_obj);
    const char *genero = json_object_get_string(genero_obj);
    const char *resumen = json_object_get_string(resumen_obj);
    int cantidad = json_object_get_int(cantidad_obj);

    // libros[i].id = id;
    strcpy(libros[i].titulo, titulo);
    strcpy(libros[i].autor, autor);
    libros[i].anio = anio;
    strcpy(libros[i].genero, genero);
    strcpy(libros[i].resumen, resumen);
    libros[i].cantidad = cantidad;
  }

  json_object_put(parsed_json);
  return libros;
}

int contarLibros(Libro *libros)
{
  int cantidadLibros = 0;
  while (strcmp(libros[cantidadLibros].titulo, "") != 0) {
    cantidadLibros++;
  }
  return cantidadLibros;
}

void mostrarLibros()
{
  Libro *libros = cargarLibros("./data/catalogo.json");
  int cantidadLibros = contarLibros(libros);
  imprimirLibros(libros, cantidadLibros);
  printf("Cantidad total de libros: %d\n", cantidadLibros);
  free(libros);

  pausar("Presione enter para volver al menú...");
  limpiarPantalla();
}

// funcion existeLibro(libros, libro)
// recibe un arreglo de libros y un libro
// retorna 1 si el libro existe en el arreglo de libros
// retorna 0 si el libro no existe en el arreglo de libros

int existeLibro(Libro *libros, Libro libro)
{
  int cantidadLibros = contarLibros(libros);
  for (int i = 0; i < cantidadLibros; i++) {
    if (strcmp(libros[i].titulo, libro.titulo) == 0) {
      return 1;
    }
  }
  return 0;
}

void guardarLibros(char *ruta, Libro *libros)
{
  struct json_object *parsed_json = json_object_new_array();

  int cantidadLibros = contarLibros(libros);

  for (int i = 0; i < cantidadLibros; i++) {
    struct json_object *obj = json_object_new_object();

    struct json_object *titulo_obj, *autor_obj, *anio_obj, *genero_obj, *resumen_obj, *cantidad_obj;

    titulo_obj = json_object_new_string(libros[i].titulo);
    autor_obj = json_object_new_string(libros[i].autor);
    anio_obj = json_object_new_int(libros[i].anio);
    genero_obj = json_object_new_string(libros[i].genero);
    resumen_obj = json_object_new_string(libros[i].resumen);
    cantidad_obj = json_object_new_int(libros[i].cantidad);

    json_object_object_add(obj, "titulo", titulo_obj);
    json_object_object_add(obj, "autor", autor_obj);
    json_object_object_add(obj, "anio", anio_obj);
    json_object_object_add(obj, "genero", genero_obj);
    json_object_object_add(obj, "resumen", resumen_obj);
    json_object_object_add(obj, "cantidad", cantidad_obj);

    json_object_array_add(parsed_json, obj);
  }

  char *contenido = (char *)strcat(json_object_to_json_string(parsed_json), "\n");
  escribirArchivo(ruta, contenido);
}

int libroValido(Libro libro)
{
  if (
      strcmp(libro.titulo, "") == 0 ||
      strcmp(libro.autor, "") == 0 ||
      strcmp(libro.genero, "") == 0 ||
      strcmp(libro.resumen, "") == 0 ||
      libro.anio == 0) {
    return 0;
  }

  char caracteresValidos[] = "abcdefghijklmnñopqrstuvwxyzABCDEFGHIJKLMNÑOPQRSTUVWXYZáéíóúÁÉÍÓÚ1234567890 .,()-¿?¡!";
  int cantidadCaracteresValidos = strlen(caracteresValidos);
  int cantidadCaracteresTitulo = strlen(libro.titulo);

  for (int i = 0; i < cantidadCaracteresTitulo; i++) {
    int caracterValido = 0;
    for (int j = 0; j < cantidadCaracteresValidos; j++) {
      if (libro.titulo[i] == caracteresValidos[j])
      {
        caracterValido = 1;
        break;
      }
    }
    if (caracterValido == 0) {
      return 0;
    }
  }

  return 1;
}

void validarCatalogo()
{
  Libro *libros = cargarLibros("./data/catalogo.json");
  int cantidadLibros = contarLibros(libros);
  // elimina los libros que no tiene anio
  for (int i = 0; i < cantidadLibros; i++) {
    if (libroValido(libros[i]) == 0) {
      for (int j = i; j < cantidadLibros; j++)
      {
        libros[j] = libros[j + 1];
      }
      cantidadLibros--;
      i--;
    }
  }

  guardarLibros("./data/catalogo.json", libros);
}

void actualizarCatalogo()
{
  char rutaArchivo[200];
  printf("Ingrese la ruta del archivo: ");
  scanf("%199s", rutaArchivo);

  char *contenido = leerArchivo(rutaArchivo);
  Libro *libros = cargarLibros("./data/catalogo.json");
  char *linea = "";
  int librosAgregados = 0;

  while (*contenido != '\0') {
    linea = strsep(&contenido, "\n");
    Libro libro;
    strcpy(libro.titulo, strsep(&linea, "|"));
    strcpy(libro.autor, strsep(&linea, "|"));
    libro.anio = atoi(strsep(&linea, "|"));
    strcpy(libro.genero, strsep(&linea, "|"));
    strcpy(libro.resumen, strsep(&linea, "|"));
    libro.cantidad = atoi(strsep(&linea, "|"));

    // si el libro no está en la lista de libros, entonces agregarlo
    if (!existeLibro(libros, libro)) {
      libros = realloc(libros, sizeof(Libro) * (contarLibros(libros) + 1));
      libros[contarLibros(libros)] = libro;
      librosAgregados++;
      printf("+ El libro %s se agregó al catalogo.\n", libro.titulo);
    }
    else
    {
      printf("- El libro %s ya existe en el catalogo.\n", libro.titulo);
    }
  }

  if (librosAgregados > 0) {
    // guardar el arreglo de libros en el archivo
    guardarLibros("./data/catalogo.json", libros);
    printf("\n===============Se agregaron %d libros al catalogo.===============\n", librosAgregados);
  }
  else {
    printf("No se agregaron libros al catalogo.\n");
  }

  validarCatalogo();

  pausar("Presione enter para volver al menú...");
  limpiarPantalla();
}

void opcionesGestionCatalogo()
{
  int opcion = menuGestionCatalogo();

  while (opcion != 5) {
    switch (opcion) {
    case 1:
      actualizarCatalogo();
      break;
    case 2:
      printf("Modificar libro.\n");
      break;
    case 3:
      printf("Eliminar libro.\n");
      break;
    case 4:
      mostrarLibros();
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

void opcionesOperativas()
{
  int opcion = menuOpcionesOperativas();
  while (opcion != 6) {
    switch (opcion) {
    case 1:
      opcionesGestionCatalogo();
      break;
    case 2:
      opcionGestionUsuarios();
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
