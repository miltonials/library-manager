#ifndef TIEMPO_H
#define TIEMPO_H

// #include <stdio.h>
#include <time.h>

time_t obtenerFechaDeString(const char *fecha);
int validarRangoFechas(const char *fechaInicio, const char *fechaFinal);
int formatoFecha(char *fecha);
int tadiasEnDias(const char *fechaInicio, const char *fechaFinal);
char *obtenerFechaActual();

char *obtenerFechaActual() {
  time_t fechaActual_time = time(NULL);
  struct tm *fechaActual_tm = localtime(&fechaActual_time);
  char *fechaActual = malloc(sizeof(char) * 11);
  strftime(fechaActual, 11, "%d/%m/%Y", fechaActual_tm);
  return fechaActual;
}

int tadiasEnDias(const char *fechaEntrega, const char *fechaActual) {
  time_t fechaEntrega_time = obtenerFechaDeString(fechaEntrega);
  time_t fechaFinal_time = obtenerFechaDeString(fechaActual);
  // si la fecha de devolucion es mayor a la fecha de fin
  if (fechaEntrega_time > fechaFinal_time) {
    int dias = (int) difftime(fechaEntrega_time, fechaFinal_time) / (60 * 60 * 24);
    return dias;
  }
  return 0;
}

time_t obtenerFechaDeString(const char *fecha) {
  struct tm fecha_tm = {0};  // Inicializar la estructura tm a 0
  char *cp_fecha = malloc(sizeof(char) * (strlen(fecha) + 1));  // Asegurar espacio para el carácter nulo

  strcpy(cp_fecha, fecha);

  // Parsear la fecha en la estructura tm
  if (strptime(cp_fecha, "%d/%m/%Y", &fecha_tm) == NULL) {
    // Manejar el error si la fecha no se pudo analizar correctamente
    // perror("Error al analizar la fecha");
    free(cp_fecha);  // Liberar la memoria antes de salir
    return -1;  // Valor de error
  }

  time_t fecha_time = mktime(&fecha_tm);
  free(cp_fecha);  // Liberar la memoria después de usarla

  return fecha_time;
}


int validarRangoFechas(const char *fechaInicio, const char *fechaFinal) {
  time_t fechaInicio_time = obtenerFechaDeString(fechaInicio);
  time_t fechaFinal_time = obtenerFechaDeString(fechaFinal);

  if (fechaInicio_time >= fechaFinal_time) {
    return 0;
  }

  return 1;
}


int formatoFecha(char *fecha) {
  //fecha con formato: dd/mm/aaaa -> 10 caracteres
  
  if (strlen(fecha) != 10) {
    return 0;
  }

  char *dia = malloc(3);
  char *mes = malloc(3);
  char *anio = malloc(5);

  dia[0] = fecha[0];
  dia[1] = fecha[1];
  dia[2] = '\0';

  mes[0] = fecha[3];
  mes[1] = fecha[4];
  mes[2] = '\0';

  anio[0] = fecha[6];
  anio[1] = fecha[7];
  anio[2] = fecha[8];
  anio[3] = fecha[9];
  anio[4] = '\0';

  int diaInt = atoi(dia);
  int mesInt = atoi(mes);
  int anioInt = atoi(anio);

  if (diaInt < 1 || diaInt > 31) {
    return 0;
  }

  if (mesInt < 1 || mesInt > 12) {
    return 0;
  }

  if (anioInt < 2021) {
    return 0;
  }

  return 1;
}


int diferenciaDias(const char *fechaInicio, const char *fechaFinal) {
  time_t fechaInicio_time = obtenerFechaDeString(fechaInicio);
  time_t fechaFinal_time = obtenerFechaDeString(fechaFinal);

  int dias = (int) difftime(fechaFinal_time, fechaInicio_time) / (60 * 60 * 24);

  return dias;
}

#endif