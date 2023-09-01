#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/**
 * @file estructuras.h
 * @brief Definicion de estructuras
 * @version 1.0
 * @date 29/09/2023
 * @author @Andyporras,@miltonials
*/

// Definicion de estructuras
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

typedef struct{
  int idLibro;
  char *nombreLibro;
  int cantidadPrestamos;
} EstadisticasLibro;

typedef struct{
  char *cedulaUsuario;
  char *nombreUsuario;
  int cantidadPrestamos;
} EstadisticasUsuario;

typedef struct{
  int idPrestamo;
  char *mesAnio;
  int montosRecaudados;
} MayorRecaudacionMes;



#endif