#ifndef ACTUALIZARDATOS_H
#define ACTUALIZARDATOS_H

/**
 * @file actualizarDatos.h
 * @brief Funciones para actualizar los datos almacenados en los archivos JSON
 * @version 1.0
 * @date 29/09/2023
 * @author @miltonials
*/

/**
 * Actualiza Biblioteca
 * 
 * Esta funcion actualiza los datos almacenados en los archivos JSON
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @param rutaArchivos Ruta de la carpeta de archivos
 * @return void
*/
void actualizarBiblioteca(Biblioteca *dirM_biblioteca, char *rutaArchivos) {
  actualizarUsuarios(dirM_biblioteca, rutaArchivos);
  actualizarCatalogo(dirM_biblioteca, rutaArchivos);
  actualizarPrestamos(dirM_biblioteca, rutaArchivos);

  printf("📚 Biblioteca actualizada correctamente.\n");
}

/**
 * Obtiene la ruta de actualizacion
 * 
 * Esta funcion obtiene la ruta de actualizacion de los archivos JSON
 * 
 * @param rutaArchivos Ruta de la carpeta de archivos
 * @param nombreArchivo Nombre del archivo
 * @return char* Ruta de actualizacion
*/
char* obtenerRutaActualizacion(char *rutaArchivos, char *nombreArchivo) {
  char *ruta = malloc(strlen(rutaArchivos) + strlen(nombreArchivo) + 1);
  strcpy(ruta, rutaArchivos);
  strcat(ruta, nombreArchivo);
  return ruta;
}

/**
 * Actualiza los usuarios
 * 
 * Esta funcion actualiza los usuarios almacenados en el archivo JSON
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @param rutaArchivos Ruta de la carpeta de archivos
 * @return void
*/
void actualizarUsuarios(Biblioteca *dirM_biblioteca, char *rutaArchivos) {
  char *ruta = obtenerRutaActualizacion(rutaArchivos, "usuarios.json");
  FILE *archivo = fopen(ruta, "w");

  if (archivo == NULL) {
    printf("🚨 Error al abrir el archivo de usuarios.\n");
    return;
  }

  json_object *usuarios = json_object_new_array();
  for (int i = 0; i < dirM_biblioteca->cantidadUsuarios; i++) {
    json_object *usuario = json_object_new_object();
    json_object_object_add(usuario, "cedula", json_object_new_string(dirM_biblioteca->usuarios[i].cedula));
    json_object_object_add(usuario, "nombre", json_object_new_string(dirM_biblioteca->usuarios[i].nombre));
    json_object_object_add(usuario, "direccion", json_object_new_string(dirM_biblioteca->usuarios[i].direccion));
    json_object_array_add(usuarios, usuario);
  }
  json_object_to_file(ruta, usuarios);
  free(ruta);
  fclose(archivo);
}

/**
 * Actualiza el catalogo
 * 
 * Esta funcion actualiza el catalogo almacenado en el archivo JSON
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @param rutaArchivos Ruta de la carpeta de archivos
 * @return void
*/
void actualizarCatalogo(Biblioteca *dirM_biblioteca, char *rutaArchivos) {
  char *ruta = obtenerRutaActualizacion(rutaArchivos, "catalogo.json");
  FILE *archivo = fopen(ruta, "w");

  if (archivo == NULL) {
    printf("🚨 Error al abrir el archivo de catalogo.\n");
    return;
  }

  json_object *catalogo = json_object_new_array();
  for (int i = 0; i < dirM_biblioteca->cantidadLibros; i++) {
    json_object *libro = json_object_new_object();
    json_object_object_add(libro, "id", json_object_new_int(dirM_biblioteca->libros[i].id));
    json_object_object_add(libro, "titulo", json_object_new_string(dirM_biblioteca->libros[i].titulo));
    //al ejecutar la linea anterior da segmentation faul ¿por qué?
    // 
    json_object_object_add(libro, "autor", json_object_new_string(dirM_biblioteca->libros[i].autor));
    json_object_object_add(libro, "anio", json_object_new_int(dirM_biblioteca->libros[i].anio));
    json_object_object_add(libro, "genero", json_object_new_string(dirM_biblioteca->libros[i].genero));
    json_object_object_add(libro, "resumen", json_object_new_string(dirM_biblioteca->libros[i].resumen));
    json_object_object_add(libro, "cantidad", json_object_new_int(dirM_biblioteca->libros[i].cantidad));
    json_object_array_add(catalogo, libro);
  }
  json_object_to_file(ruta, catalogo);
  free(ruta);
  fclose(archivo);
}

/**
 * Actualiza los prestamos
 * 
 * Esta funcion actualiza los prestamos almacenados en el archivo JSON
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @param rutaArchivos Ruta de la carpeta de archivos
 * @return void
*/
void actualizarPrestamos(Biblioteca *dirM_biblioteca, char *rutaArchivos) {
  char *ruta = obtenerRutaActualizacion(rutaArchivos, "prestamos.json");
  FILE *archivo = fopen(ruta, "w");
  
  if (archivo == NULL) {
    printf("🚨 Error al abrir el archivo de prestamos.\n");
    return;
  }

  json_object *prestamos = json_object_new_array();
  for (int i = 0; i < dirM_biblioteca->cantidadPrestamos; i++) {
    json_object *prestamo = json_object_new_object();
    json_object_object_add(prestamo, "id", json_object_new_int(dirM_biblioteca->prestamos[i].id));
    // json_object_object_add(prestamo, "tituloLibro", json_object_new_string(dirM_biblioteca->prestamos[i].tituloLibro));
    json_object_object_add(prestamo, "idLibro", json_object_new_int(dirM_biblioteca->prestamos[i].idLibro));
    json_object_object_add(prestamo, "cedulaUsuario", json_object_new_string(dirM_biblioteca->prestamos[i].cedulaUsuario));
    json_object_object_add(prestamo, "fechaInicio", json_object_new_string(dirM_biblioteca->prestamos[i].fechaInicio));
    json_object_object_add(prestamo, "fechaFin", json_object_new_string(dirM_biblioteca->prestamos[i].fechaFin));
    json_object_object_add(prestamo, "fechaDevolucion", json_object_new_string(dirM_biblioteca->prestamos[i].fechaDevolucion));
    json_object_object_add(prestamo, "estado", json_object_new_int(dirM_biblioteca->prestamos[i].estado));
    json_object_array_add(prestamos, prestamo);
  }
  json_object_to_file(ruta, prestamos);
  
  free(ruta);
  fclose(archivo);
}


#endif