#ifndef TIEMPO_H
#define TIEMPO_H

// #include <stdio.h>
#include <time.h>
/**
 * @file tiempo.h
 * @brief Funciones para gestionar el tiempo
 * @version 1.0
 * @date 29/09/2023
 * @author @Andyporras, @miltonials
*/

time_t obtenerFechaDeString(const char *fecha);
int validarRangoFechas(const char *fechaInicio, const char *fechaFinal);
int formatoFecha(char *fecha);
int tadiasEnDias(const char *fechaInicio, const char *fechaFinal);
char *obtenerFechaActual();

/**
 * Obtener fecha actual.
 * 
 * Esta funcion cactura la fecha actual del sistema y la retorna.
 *
 * @return fecha actual del sistema. Ejemplo: 01/01/2021
 */
char *obtenerFechaActual() {
  time_t fechaActual_time = time(NULL);
  struct tm *fechaActual_tm = localtime(&fechaActual_time);
  char *fechaActual = malloc(sizeof(char) * 11);
  strftime(fechaActual, 11, "%d/%m/%Y", fechaActual_tm);
  return fechaActual;
}

/**
 * dias de atraso en dias.
 * 
 * Esta funcion calcula los dias de atraso en dias de una fecha de entrega.
 * 
 * @param fechaEntrega fecha de entrega del libro
 * @param fechaActual fecha actual del sistema
 * @return dias de atraso en dias
*/
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

/**
 * Obtener fecha de string.
 * 
 * Esta funcion convierte una fecha en formato dd/mm/aaaa a un time_t. Retorna -1 si la fecha no se pudo analizar correctamente.
 * 
 * @param fecha fecha en formato dd/mm/aaaa
 * @return fecha en formato time_t
*/
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

/**
 * Validar rango de fechas.
 * 
 * Esta funcion valida que la fecha de inicio sea menor a la fecha final.
 * 
 * @param fechaInicio fecha de inicio
 * @param fechaFinal fecha final
 * 
 * @return 1 si la fecha de inicio es menor a la fecha final, 0 en caso contrario
*/
int validarRangoFechas(const char *fechaInicio, const char *fechaFinal) {
  time_t fechaInicio_time = obtenerFechaDeString(fechaInicio);
  time_t fechaFinal_time = obtenerFechaDeString(fechaFinal);

  if (fechaInicio_time >= fechaFinal_time) {
    return 0;
  }

  return 1;
}

/**
 * Formato fecha.
 * 
 * Esta funcion valida que la fecha tenga el formato correcto. Ejemplo: 01/01/2021
 * 
 * @param fecha fecha a validar
 * @return 1 si la fecha tiene el formato correcto, 0 en caso contrario
*/
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

/**
 * Diferencia dias.
 * 
 * Esta funcion calcula la diferencia en dias entre dos fechas.
 * 
 * @param fechaInicio fecha de inicio
 * @param fechaFinal fecha final
 * @return diferencia en dias entre las dos fechas
*/  
int diferenciaDias(const char *fechaInicio, const char *fechaFinal) {
  time_t fechaInicio_time = obtenerFechaDeString(fechaInicio);
  time_t fechaFinal_time = obtenerFechaDeString(fechaFinal);

  if (fechaInicio_time == -1 || fechaFinal_time == -1) {
    // Manejar errores en la conversión de fechas
    return -1;  // Valor de error
  }

  // Calcular la diferencia en segundos entre las dos fechas
  time_t diferencia_segundos = fechaFinal_time - fechaInicio_time;

  // Convertir la diferencia de segundos a días (86400 segundos por día)
  int diferencia_dias = diferencia_segundos / 86400;

  return diferencia_dias;
}

#endif