#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct {
  int cedula;
  char nombre[50];
  char direccion[50];
} Usuario;

typedef struct {
  char titulo[50];
  char autor[50];
  int anio;
  char genero[50];
  char resumen[500];
  int cantidad;
} Libro;

typedef struct {
  int id;
  int idLibro;
  int cedulaUsuario;
  char fechaInicio[50];
  char fechaFin[50];
} Prestamo;

typedef struct{
  int cedula;
  char titulo[50];
  char fechaInicio[50];
  char fechaFinal[50];
} PrestamoEjemplar;

#endif