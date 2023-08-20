#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct {
  int cedula;
  char nombre[50];
  char direccion[50];
} Usuario;

typedef struct {
  int id;
  char titulo[50];
  char autor[50];
  int anio;
  char genero[50];
  char resumen[100];
  int cantidad;
} Libro;

typedef struct {
  int id;
  int idLibro;
  int cedulaUsuario;
  char fechaInicio[50];
  char fechaFin[50];
} Prestamo;

#endif