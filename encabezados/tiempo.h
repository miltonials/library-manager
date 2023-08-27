#ifndef TIEMPO_H
#define TIEMPO_H

// #include <stdio.h>
#include <time.h>

time_t obtenerFechaDeString(const char *fecha);
int validarRangoFechas(const char *fechaInicio, const char *fechaFinal);
int formatoFecha(char *fecha);

time_t obtenerFechaDeString(const char *fecha) {
  struct tm fecha_tm;
  char *cp_fecha = malloc (sizeof(char) * strlen(fecha));
  strcpy(cp_fecha, fecha);

  strftime(cp_fecha,sizeof(cp_fecha), "%d/%m/%Y", &fecha_tm);
  strptime(fecha, "%d/%m/%Y", &fecha_tm);

  time_t fecha_time = mktime(&fecha_tm);
  // printf("Fecha en time_t: %ld\n", (long)fecha_time);

  return fecha_time;
}

int validarRangoFechas(const char *fechaInicio, const char *fechaFinal) {
  time_t fechaInicio_time = obtenerFechaDeString(fechaInicio);
  time_t fechaFinal_time = obtenerFechaDeString(fechaFinal);

  if (fechaInicio_time >= fechaFinal_time) {
    // printf("La fecha de inicio no puede ser mayor a la fecha final\n");
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


#endif