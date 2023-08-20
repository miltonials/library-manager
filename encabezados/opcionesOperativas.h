#ifndef OPCIONESOPERATIVAS_H
#define OPCIONESOPERATIVAS_H


Libro solicitarDatosLibro(){
  Libro libro;
  printf("Ingrese el titulo del libro: ");
  scanf("%49[^\n]", libro.titulo);  // Leer cadena con espacios (máximo 49 caracteres)
  getchar();  // Consumir el carácter de nueva línea pendiente

  printf("Ingrese el autor del libro: ");
  scanf("%49[^\n]", libro.autor);
  getchar();

  printf("Ingrese el anio de publicacion del libro: ");
  scanf("%d", &libro.anio);
  getchar();

  printf("Ingrese el genero del libro: ");
  scanf("%49[^\n]", libro.genero);
  getchar();

  printf("Ingrese el resumen del libro: ");
  scanf("%99[^\n]", libro.resumen);
  getchar();

  printf("Ingrese la cantidad de ejemplares del libro: ");
  scanf("%d", &libro.cantidad);

  return libro;  // Devolver la estructura Libro
}

void imprimirLibros(Libro *libros, int cantidadLibros){
  printf("Libros:\n");
  for (int i = 0; i < cantidadLibros-1; i++){
    printf("Libro %d:\n", i);
    printf("id: %d\n", libros[i].id);
    printf("titulo: %s\n", libros[i].titulo);
    printf("autor: %s\n", libros[i].autor);
    printf("anio: %d\n", libros[i].anio);
    printf("genero: %s\n", libros[i].genero);
    printf("resumen: %s\n", libros[i].resumen);
    printf("cantidad: %d\n", libros[i].cantidad);
  }
}

Libro *cargarLibros(char *rutaArchivo){
  FILE *fp;
  char buffer[5000];
  struct json_object *parsed_json;
  size_t n, i;

  fp = fopen(rutaArchivo, "r");
  fread(buffer, sizeof(buffer), 1, fp);
  fclose(fp);

  parsed_json = json_tokener_parse(buffer);
  if (parsed_json == NULL){
    printf("Error al parsear el archivo\n");
    return NULL;
  }

  n = json_object_array_length(parsed_json);
  Libro *libros = malloc(sizeof(Libro) * n);
  for (i = 0; i < n; i++){
    struct json_object *obj = json_object_array_get_idx(parsed_json, i);

    struct json_object *id_obj, *titulo_obj, *autor_obj, *anio_obj, *genero_obj, *resumen_obj, *cantidad_obj;

    json_object_object_get_ex(obj, "id", &id_obj);
    json_object_object_get_ex(obj, "titulo", &titulo_obj);
    json_object_object_get_ex(obj, "autor", &autor_obj);
    json_object_object_get_ex(obj, "anio", &anio_obj);
    json_object_object_get_ex(obj, "genero", &genero_obj);
    json_object_object_get_ex(obj, "resumen", &resumen_obj);
    json_object_object_get_ex(obj, "cantidad", &cantidad_obj);

    int id = json_object_get_int(id_obj);
    const char *titulo = json_object_get_string(titulo_obj);
    const char *autor = json_object_get_string(autor_obj);
    int anio = json_object_get_int(anio_obj);
    const char *genero = json_object_get_string(genero_obj);
    const char *resumen = json_object_get_string(resumen_obj);
    int cantidad = json_object_get_int(cantidad_obj);

    libros[i].id = id;
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

/*
se debe permitir incluir en lote (incluye con información correcta y únicamente nuevos);
la información será el nombre, autor, año publicación, género, resumen y cantidad. La unicidad la dará el nombre. Se debe indicar una ruta de archivo que contendrá los registros por incluir, con la siguiente forma:
Cien años de soledad|Gabriel García Márquez|1967|Realismo mágico|Esta novela cuenta la historia|4 1984|George Orwell|1949|Distopía|En un futuro totalitario, el protagonista lucha contra el régimen|3 Orgullo y prejuicio|Jane Austen|1813|Novela romántica|La historia sigue Elizabeth Bennet mientras|2
Según la cantidad indicada se genera un identificar para cada ejemplar (se almacenan ejemplares
de producciones bibliográficas). Los datos se almacenan en otro archivo tipo json.*/
int agregarLibro(){
  // lista de struct libro, se carga desde el archivo
  Libro *libros = NULL;
  Libro libroAgregar;
  libroAgregar = solicitarDatosLibro();
  // solicitar ruta de archivo a cargar
  char rutaArchivo[100];
  printf("Ingrese la ruta del archivo: ");
  scanf("%s", rutaArchivo);
  // cargar archivo en lista de struct libro
  libros = cargarLibros(rutaArchivo);
  // se calcula la cantidad de libros


  return 5;
}

int contarLibros(Libro *libros){
  int cantidadLibros = 0;
  while (libros[cantidadLibros].id != 0){
    cantidadLibros++;
  }
  return cantidadLibros;
}

int mostrarLibros(){
  // cargar archivo en lista de struct libro
  Libro *libros = cargarLibros("catalogo.json");
  // se calcula la cantidad de libros
  int cantidadLibros = contarLibros(libros);
  printf("cantidadLibros: %d\n", cantidadLibros);
  imprimirLibros(libros, cantidadLibros);
  free(libros);
  return 5;
}

void opcionesGestionCatalogo() {
  int opcion = menuGestionCatalogo();
  printf("opcion: %d\n", opcion);

  while (opcion != 5){
    int temp =  opcion;
    switch (opcion){
      case 1:
        printf("Agregar libro.\n");
        agregarLibro();
        opcion = temp;
        break;
      case 2:
        printf("Modificar libro.\n");
        break;
      case 3:
        printf("Eliminar libro.\n");
        break;
      case 4:
        printf("Buscar libro.\n");
        mostrarLibros();
        opcion = temp;
        break;
      case 5:
        printf("Volver.\n");
        break;
      default:
        printf("Opcion invalida.\n");
        break;
    }
  }
}

int opcionesOperativas() {
  int opcion = menuOpcionesOperativas();
  while (opcion != 6){
    int temp =  opcion;
    switch (opcion){
      case 1:
        opcionesGestionCatalogo();
        opcion = temp;
        break;
      case 2:
        printf("Gestion de usuarios.\n");
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
  }
  return 0;
}


#endif // OPCIONESOPERATIVAS_H
