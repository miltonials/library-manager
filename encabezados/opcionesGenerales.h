#ifndef OPCIONESGENERALES_H
#define OPCIONESGENERALES_H
/*
Préstamo de ejemplar El sistema debe permitir incluir un préstamo de ejemplar, 
el usuario deberá indicar usuario, el identificador de ejemplar, una fecha de 
inicio y final. El sistema verificará que para las fechas indicadas el ejemplar 
esté disponible, si existe y está disponible, se le generará un comprobante 
indicando: identificador de préstamo, usuario, fecha inicio, fecha de entrega,
nombre e identificador de ejemplar.*/
PrestamoEjemplar *cargarPrestamos(char *rutaArchivo){
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
  PrestamoEjemplar *prestamoEjemplares = malloc(sizeof(PrestamoEjemplar) * n);
  for(i = 0; i < n; i++){
    struct json_object *obj = json_object_array_get_idx(parsed_json, i);

    struct json_object *cedula_obj, *titulo_obj, *fechaInicio_obj, *fechaFinal_obj;

    json_object_object_get_ex(obj, "cedula", &cedula_obj);
    json_object_object_get_ex(obj, "titulo", &titulo_obj);
    json_object_object_get_ex(obj, "fechaInicio", &fechaInicio_obj);
    json_object_object_get_ex(obj, "fechaFinal", &fechaFinal_obj);

    int cedula = json_object_get_int(cedula_obj);
    const char *titulo = json_object_get_string(titulo_obj);
    const char *fechaInicio = json_object_get_string(fechaInicio_obj);
    const char *fechaFinal = json_object_get_string(fechaFinal_obj);

    prestamoEjemplares[i].cedula = cedula;
    strcpy(prestamoEjemplares[i].titulo, titulo);
    strcpy(prestamoEjemplares[i].fechaInicio, fechaInicio);
    strcpy(prestamoEjemplares[i].fechaFinal, fechaFinal);

  }
  json_object_put(parsed_json);
  return prestamoEjemplares;
}

int contarPrestamos(char *rutaArchivo){
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
    return -1;
  }

  n = json_object_array_length(parsed_json);
  return n;
}

void guardarPrestamo(PrestamoEjemplar nuevoPrestamo, PrestamoEjemplar *prestamoEjemplares){
  FILE *fp;
  char buffer[5000];
  struct json_object *parsed_json;
  struct json_object *json_prestamos;
  struct json_object *json_prestamo;
  struct json_object *json_cedula, *json_titulo, *json_fechaInicio, *json_fechaFinal;
  size_t n, i;

  fp = fopen("./data/prestamos.json", "r");
  fread(buffer, sizeof(buffer), 1, fp);
  fclose(fp);

  parsed_json = json_tokener_parse(buffer);
  if (parsed_json == NULL){
    printf("Error al parsear el archivo\n");
    return;
  }

  n = json_object_array_length(parsed_json);
  json_prestamos = json_object_new_array();
  for (int i = 0; i < n; i++)
  {
    json_prestamo = json_object_new_object();

    json_cedula = json_object_new_int(prestamoEjemplares[i].cedula);
    json_object_object_add(json_prestamo, "cedula", json_cedula);

    json_titulo = json_object_new_string(prestamoEjemplares[i].titulo);
    json_object_object_add(json_prestamo, "titulo", json_titulo);

    json_fechaInicio = json_object_new_string(prestamoEjemplares[i].fechaInicio);
    json_object_object_add(json_prestamo, "fechaInicio", json_fechaInicio);

    json_fechaFinal = json_object_new_string(prestamoEjemplares[i].fechaFinal);
    json_object_object_add(json_prestamo, "fechaFinal", json_fechaFinal);
    
    json_object_array_add(json_prestamos, json_prestamo);
  }
  json_prestamo = json_object_new_object();

  json_cedula = json_object_new_int(nuevoPrestamo.cedula);
  json_object_object_add(json_prestamo, "cedula", json_cedula);

  json_titulo = json_object_new_string(nuevoPrestamo.titulo);
  json_object_object_add(json_prestamo, "titulo", json_titulo);

  json_fechaInicio = json_object_new_string(nuevoPrestamo.fechaInicio);
  json_object_object_add(json_prestamo, "fechaInicio", json_fechaInicio);

  json_fechaFinal = json_object_new_string(nuevoPrestamo.fechaFinal);
  json_object_object_add(json_prestamo, "fechaFinal", json_fechaFinal);

  json_object_array_add(json_prestamos, json_prestamo);

  fp = fopen("./data/prestamos.json", "w");
  json_object_to_file_ext("./data/prestamos.json", json_prestamos, JSON_C_TO_STRING_PRETTY);
  fclose(fp);
  json_object_put(parsed_json);
  printf("Prestamo registrado con exito\n");
  free(prestamoEjemplares);
}


void generarComprobante(int cedula, char *titulo, char *fechaInicio, char *fechaFinal){
  //validar que este disponible el libro
  PrestamoEjemplar *prestamoEjemplares = cargarPrestamos("./data/prestamos.json");
  Libro *libros = cargarLibros("./data/catalogo.json");
  int cantidadLibros = contarLibros(libros);
  int cantidadPrestamos = contarPrestamos("./data/prestamos.json");
  int libroDisponible = 0;
  for (int i = 0; i < cantidadLibros; i++)
  {
    if (strcmp(libros[i].titulo, titulo) == 0){
      libroDisponible = libros[i].cantidad;
    }
  }
  for (int i = 0; i < cantidadPrestamos; i++)
  {
    if (strcmp(prestamoEjemplares[i].titulo, titulo) == 0){
      libroDisponible--;
    }
  }
  if(libroDisponible > 0){
    //generar comprobante
    printf("El libro esta disponible\n");
    printf("El comprobante es:\n");
    printf("Cedula: %d\n", cedula);
    printf("Titulo: %s\n", titulo);
    printf("Fecha de inicio: %s\n", fechaInicio);
    printf("Fecha final: %s\n", fechaFinal);
    //guardar el prestamo en el archivo json
    PrestamoEjemplar nuevoPrestamo;
    nuevoPrestamo.cedula = cedula;
    strcpy(nuevoPrestamo.titulo, titulo);
    strcpy(nuevoPrestamo.fechaInicio, fechaInicio);
    strcpy(nuevoPrestamo.fechaFinal, fechaFinal);
    guardarPrestamo(nuevoPrestamo, prestamoEjemplares);

  }else{
    printf("El libro no esta disponible\n");
  }
}



void prestamoEjemplar(){
  Usuario *usuarios = cargarUsuarios("./data/usuarios.json");
  Libro *libros = cargarLibros("./data/catalogo.json");
  int cantidadLibros = contarLibros(libros);
  int cantidadUsuarios = contarUsuarios("data/usuarios.json");
  int cedula;
  char titulo[50];
  char fechaInicio[20];
  char fechaFinal[20];
  int existeUsuario = 0;
  int existeLibro = 0;
  printf("Ingrese la cedula del usuario: ");
  scanf("%d", &cedula);
  printf("Ingrese el titulo del libro: ");
  scanf("%s", titulo);
  printf("Ingrese la fecha de inicio: ");
  scanf("%s", fechaInicio);
  printf("Ingrese la fecha final: ");
  scanf("%s", fechaFinal);
  for (int i = 0; i < cantidadUsuarios; i++)
  {
    if (usuarios[i].cedula == cedula)
    {
      existeUsuario = 1;
    }
  }
  for (int i = 0; i < cantidadLibros; i++)
  {
    if(strcmp(libros[i].titulo, titulo) == 0){
      existeLibro = 1;
    }
  }
  if (existeUsuario == 1 && existeLibro == 1)
  {
    printf("El usuario y el libro existen\n");
    //generar comprobante
    generarComprobante(cedula, titulo, fechaInicio, fechaFinal);
  }
  else if (existeUsuario == 0 && existeLibro == 1)
  {
    printf("El usuario no existe\n");
    prestamoEjemplar();
  }
  else if (existeUsuario == 1 && existeLibro == 0)
  {
    printf("El libro no existe\n");
    prestamoEjemplar();
  }
  else
  {
    printf("El usuario y el libro no existen\n");
    prestamoEjemplar();
  }
}

void opcionesGenerales() {
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
      prestamoEjemplar();
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