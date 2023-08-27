#ifndef CARGADATOS_H
#define CARGADATOS_H

void cargarBiblioteca(Biblioteca *dirM_biblioteca, char *rutaArchivos);
void cargarUsuarios2(Biblioteca *dirM_biblioteca);
void cargarLibros2(Biblioteca *dirM_biblioteca);
void cargarPrestamos2(Biblioteca *dirM_biblioteca);
void resumenBiblioteca(Biblioteca *dirM_biblioteca);

void actualizarBiblioteca(Biblioteca *dirM_biblioteca, char *rutaArchivos);
void actualizarUsuarios(Biblioteca *dirM_biblioteca, char *rutaArchivos);
void actualizarCatalogo(Biblioteca *dirM_biblioteca, char *rutaArchivos);
void actualizarPrestamos(Biblioteca *dirM_biblioteca, char *rutaArchivos);

void cargarBiblioteca(Biblioteca *dirM_biblioteca, char *rutaArchivos) {
  dirM_biblioteca->rutaArchivos = rutaArchivos;
  cargarUsuarios2(dirM_biblioteca);
  cargarLibros2(dirM_biblioteca);
  cargarPrestamos2(dirM_biblioteca);

  printf("📚 Biblioteca cargada correctamente.\n");
}

void cargarUsuarios2(Biblioteca *dirM_biblioteca) {
  char *ruta = malloc(strlen(dirM_biblioteca->rutaArchivos) + strlen("usuarios.json") + 1);
  strcpy(ruta, dirM_biblioteca->rutaArchivos);
  strcat(ruta, "usuarios.json");
  char *contenidoArchivo = leerArchivo(ruta);
  
  if (contenidoArchivo == NULL || strlen(contenidoArchivo) == 0) {
    escribirArchivo(ruta, "[]");
    contenidoArchivo = leerArchivo(ruta);
  }
  
  free(ruta);

  json_object *usuarios = json_tokener_parse(contenidoArchivo);
  dirM_biblioteca->cantidadUsuarios = json_object_array_length(usuarios);
  dirM_biblioteca->usuarios = malloc(sizeof(Usuario) * dirM_biblioteca->cantidadUsuarios);

  for (int i = 0; i < dirM_biblioteca->cantidadUsuarios; i++) {
    json_object *usuario = json_object_array_get_idx(usuarios, i);
    json_object *cedula = json_object_object_get(usuario, "cedula");
    json_object *nombre = json_object_object_get(usuario, "nombre");
    json_object *direccion = json_object_object_get(usuario, "direccion");

    dirM_biblioteca->usuarios[i].cedula = malloc(strlen(json_object_get_string(cedula)) + 1);
    dirM_biblioteca->usuarios[i].nombre = malloc(strlen(json_object_get_string(nombre)) + 1);
    dirM_biblioteca->usuarios[i].direccion = malloc(strlen(json_object_get_string(direccion)) + 1);
    strcpy(dirM_biblioteca->usuarios[i].cedula, json_object_get_string(cedula));
    strcpy(dirM_biblioteca->usuarios[i].nombre, json_object_get_string(nombre));
    strcpy(dirM_biblioteca->usuarios[i].direccion, json_object_get_string(direccion));
  }

  free(contenidoArchivo);
  printf("👤 Usuarios cargados correctamente.\n");
}

void cargarLibros2(Biblioteca *dirM_biblioteca) {
  char *ruta = malloc(strlen(dirM_biblioteca->rutaArchivos) + strlen("catalogo.json") + 1);
  strcpy(ruta, dirM_biblioteca->rutaArchivos);
  strcat(ruta, "catalogo.json");
  char *contenidoArchivo = leerArchivo(ruta);
  
  if (contenidoArchivo == NULL || strlen(contenidoArchivo) == 0) {
    escribirArchivo(ruta, "[]");
    contenidoArchivo = leerArchivo(ruta);
  }
  
  free(ruta);

  json_object *libros = json_tokener_parse(contenidoArchivo);
  dirM_biblioteca->cantidadLibros = json_object_array_length(libros);
  dirM_biblioteca->libros = malloc(sizeof(Libro) * dirM_biblioteca->cantidadLibros);

  for (int i = 0; i < dirM_biblioteca->cantidadLibros; i++){
    json_object *libro = json_object_array_get_idx(libros, i);
    json_object *titulo = json_object_object_get(libro, "titulo");
    json_object *autor = json_object_object_get(libro, "autor");
    json_object *anio = json_object_object_get(libro, "anio");
    json_object *genero = json_object_object_get(libro, "genero");
    json_object *resumen = json_object_object_get(libro, "resumen");
    json_object *cantidad = json_object_object_get(libro, "cantidad");

    dirM_biblioteca->libros[i].titulo = malloc(strlen(json_object_get_string(titulo)) + 1);
    dirM_biblioteca->libros[i].autor = malloc(strlen(json_object_get_string(autor)) + 1);
    dirM_biblioteca->libros[i].genero = malloc(strlen(json_object_get_string(genero)) + 1);
    dirM_biblioteca->libros[i].resumen = malloc(strlen(json_object_get_string(resumen)) + 1);
    
    strcpy(dirM_biblioteca->libros[i].titulo, json_object_get_string(titulo));
    strcpy(dirM_biblioteca->libros[i].autor, json_object_get_string(autor));
    dirM_biblioteca->libros[i].anio = json_object_get_int(anio);
    strcpy(dirM_biblioteca->libros[i].genero, json_object_get_string(genero));
    strcpy(dirM_biblioteca->libros[i].resumen, json_object_get_string(resumen));
    dirM_biblioteca->libros[i].cantidad = json_object_get_int(cantidad);
  }

  free(contenidoArchivo);
  printf("📖 Libros cargados correctamente.\n");
}

void cargarPrestamos2(Biblioteca *dirM_biblioteca) {
  char *ruta = malloc(strlen(dirM_biblioteca->rutaArchivos) + strlen("prestamos.json") + 1);
  strcpy(ruta, dirM_biblioteca->rutaArchivos);
  strcat(ruta, "prestamos.json");
  char *contenidoArchivo = leerArchivo(ruta);
  
  if (contenidoArchivo == NULL || strlen(contenidoArchivo) == 0) {
    escribirArchivo(ruta, "[]");
    contenidoArchivo = leerArchivo(ruta);
  }
  
  free(ruta);

  json_object *prestamos = json_tokener_parse(contenidoArchivo);
  dirM_biblioteca->cantidadPrestamos = json_object_array_length(prestamos);
  dirM_biblioteca->prestamos = malloc(sizeof(Prestamo) * dirM_biblioteca->cantidadPrestamos);

  for (int i = 0; i < dirM_biblioteca->cantidadPrestamos; i++){
    //titulo y cedula son punteros
    json_object *prestamo = json_object_array_get_idx(prestamos, i);
    json_object *id = json_object_object_get(prestamo, "id");
    json_object *tituloLibro = json_object_object_get(prestamo, "tituloLibro");
    json_object *cedulaUsuario = json_object_object_get(prestamo, "cedulaUsuario");
    json_object *fechaInicio = json_object_object_get(prestamo, "fechaInicio");
    json_object *fechaFin = json_object_object_get(prestamo, "fechaFin");
    json_object *fechaDevolucion = json_object_object_get(prestamo, "fechaDevolucion");
    json_object *estado = json_object_object_get(prestamo, "estado");
    
    dirM_biblioteca->prestamos[i].id = json_object_get_int(id);
    dirM_biblioteca->prestamos[i].tituloLibro = malloc(strlen(json_object_get_string(tituloLibro)) + 1);
    strcpy(dirM_biblioteca->prestamos[i].tituloLibro, json_object_get_string(tituloLibro));
    dirM_biblioteca->prestamos[i].cedulaUsuario = malloc(strlen(json_object_get_string(cedulaUsuario)) + 1);
    strcpy(dirM_biblioteca->prestamos[i].cedulaUsuario, json_object_get_string(cedulaUsuario));
    
    dirM_biblioteca->prestamos[i].fechaInicio = malloc(strlen(json_object_get_string(fechaInicio)) + 1);
    strcpy(dirM_biblioteca->prestamos[i].fechaInicio, json_object_get_string(fechaInicio));

    dirM_biblioteca->prestamos[i].fechaFin = malloc(strlen(json_object_get_string(fechaFin)) + 1);
    strcpy(dirM_biblioteca->prestamos[i].fechaFin, json_object_get_string(fechaFin));

    dirM_biblioteca->prestamos[i].fechaDevolucion = malloc(strlen(json_object_get_string(fechaDevolucion)));
    strcpy(dirM_biblioteca->prestamos[i].fechaDevolucion, json_object_get_string(fechaDevolucion));
    dirM_biblioteca->prestamos[i].estado = json_object_get_int(estado);
  }

  free(contenidoArchivo);
  printf("📚 Prestamos cargados correctamente.\n");
}



void resumenBiblioteca(Biblioteca *dirM_biblioteca) {
  printf("📚 Biblioteca:\n");
  printf("  📖 Libros: %d\n", dirM_biblioteca->cantidadLibros);
  printf("  👤 Usuarios: %d\n", dirM_biblioteca->cantidadUsuarios);
  printf("  📚 Prestamos: %d\n", dirM_biblioteca->cantidadPrestamos);

  printf("\n");
  printf("👤 Usuarios:\n");
  for (int i = 0; i < dirM_biblioteca->cantidadUsuarios; i++) {
    //mostrar todos lo datos en una sola linea
    printf("  %s %s %s\n", dirM_biblioteca->usuarios[i].cedula, dirM_biblioteca->usuarios[i].nombre, dirM_biblioteca->usuarios[i].direccion);
  }

  printf("\n");
  printf("📖 Libros:\n");
  for (int i = 0; i < dirM_biblioteca->cantidadLibros; i++) {
    //mostrar todos lo datos en una sola linea
    printf("  %s %s %d %s %s %d\n", dirM_biblioteca->libros[i].titulo, dirM_biblioteca->libros[i].autor, dirM_biblioteca->libros[i].anio, dirM_biblioteca->libros[i].genero, dirM_biblioteca->libros[i].resumen, dirM_biblioteca->libros[i].cantidad);
  }

  printf("\n");
  printf("📚 Prestamos:\n");
  for (int i = 0; i < dirM_biblioteca->cantidadPrestamos; i++){
    printf("  %d %s %s %s %s %d\n", dirM_biblioteca->prestamos[i].id, dirM_biblioteca->prestamos[i].tituloLibro, dirM_biblioteca->prestamos[i].cedulaUsuario, dirM_biblioteca->prestamos[i].fechaInicio, dirM_biblioteca->prestamos[i].fechaFin, dirM_biblioteca->prestamos[i].estado);
  }
}



#endif