#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct {
  char *cedula;
  char nombre[50];
  char direccion[50];
} Usuario;

typedef struct {
  char *titulo;
  char *autor;
  int anio;
  char *genero;
  char *resumen;
  int cantidad;
} Libro;

typedef struct {
  int id;
  char *tituloLibro;
  char *cedulaUsuario;
  char *fechaInicio;
  char *fechaFin;
  int estado;
} Prestamo;

typedef struct{
  char *cedula;
  char titulo[50];
  char fechaInicio[50];
  char fechaFinal[50];
  int estado;
} PrestamoEjemplar;

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