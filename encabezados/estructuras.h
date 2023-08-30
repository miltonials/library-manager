#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct {
  char *cedula;
  char *nombre;
  char *direccion;
} Usuario;

typedef struct {
  int id;
  char *titulo;
  char *autor;
  int anio;
  char *genero;
  char *resumen;
  int cantidad;
} Libro;

typedef struct {
  int id;
  int idLibro;
  char *cedulaUsuario;
  char *fechaInicio;
  char *fechaFin;
  char *fechaDevolucion;
  int estado;
} Prestamo;

typedef struct {
  Usuario *usuarios;
  Libro *libros;
  Prestamo *prestamos;
  
  char *rutaArchivos;
  int cantidadUsuarios;
  int cantidadLibros;
  int cantidadPrestamos;
} Biblioteca;



#endif