#ifndef MENUS_H
#define MENUS_H
/**
 * @file menus.h
 * @brief Funciones para mostrar los menus del programa
 * @version 1.0
 * @date 29/09/2023
 * @author @Andyporras,@miltonials
*/

/**
 * Limpiar pantalla.
 * 
 * Funcion que limpia la pantalla de la consola.
*/
void limpiarPantalla() {
  system("clear");
}

/**
 * Seleccion de opcion.
 * 
 * Funcion que lee la opcion ingresada por el usuario y la valida.
 * 
 * @return opcion ingresada por el usuario.
*/
int seleccionOpcion() {
  int opcion = 0;

  if (scanf("%d", &opcion) != 1) {
    printf("Opcion invalida. Ingrese un numero.\n");

    while (getchar() != '\n'); // Clear input buffer

    return seleccionOpcion();
  }
  limpiarPantalla();
  return opcion;
}

/**
 * Pausar.
 * 
 * Funcion que pausa la ejecucion del programa hasta que el usuario presione una tecla.
 * 
 * @param mensaje Mensaje a mostrar al usuario.
 * @return opcion ingresada por el usuario.
*/
void pausar(const char* mensaje) {
  printf("%s", mensaje);
  while (getchar() != '\n'); // Clear input buffer
  getchar();
}

/**
 * Mostrar menu principal.
 * 
 * Funcion que muestra el menu principal.
 * 
 * @return opcion ingresada por el usuario.
*/
int menuPrincipal() {
  printf("************************************************\n");
  printf("*Bienvenido al sistema de gestion de biblioteca*\n");
  printf("************************************************\n");

  printf("1. Opciones operativas.\n");
  printf("2. Opciones generales.\n");
  printf("3. Resumen de la biblioteca.\n");
  printf("4. Salir.\n");
  printf("Ingrese una opcion: ");

  return seleccionOpcion();
}

/**
 * Mostrar menu de opciones operativas.
 * 
 * Funcion que muestra el menu de opciones operativas.
 * 
 * @return opcion ingresada por el usuario.
*/
int menuOpcionesOperativas() {
  printf("*********************\n");
  printf("*Opciones Operativas*\n");
  printf("*********************\n");

  printf("1. Gestion de catalogo.\n");
  printf("2. Gestion de usuarios.\n");
  printf("3. Historial de prestamos.\n");
  printf("4. Vencimiento de prestamos.\n");
  printf("5. Estadisticas.\n");
  printf("6. Volver.\n");
  printf("Ingrese una opcion: ");

  return seleccionOpcion();
}

/**
 * Mostrar menu de opciones generales.
 * 
 * Funcion que muestra el menu de opciones generales.
 * 
 * @return opcion ingresada por el usuario.
*/
int menuOpcionesGenerales() {
  printf("********************\n");
  printf("*Opciones Generales*\n");
  printf("********************\n");

  printf("1. Busqueda simple.\n");
  printf("2. Busqueda avanzada.\n");
  printf("3. Prestamo de ejemplar.\n");
  printf("4. Devolucion de ejemplar.\n");
  printf("5. Volver.\n");
  printf("Ingrese una opcion: ");
  return seleccionOpcion();
}

/**
 * Mostrar menu gestion de catalogo.
 * 
 * Funcion que muestra el menu de gestion de catalogo.
 * 
 * @return opcion ingresada por el usuario.
*/
int menuGestionCatalogo(){
  printf("*********************\n");
  printf("*Gestion de Catalogo*\n");
  printf("*********************\n");
  printf("1. Agregar libros.\n");
  // printf("2. Modificar libro.\n");
  // printf("3. Eliminar libro.\n");
  printf("2. ver Libros.\n");
  printf("3. Volver.\n");
  printf("Ingrese una opcion: ");
  return seleccionOpcion();
}

/**
 * Mostrar menu gestion de usuarios.
 * 
 * Funcion que muestra el menu de gestion de usuarios.
 * 
 * @return opcion ingresada por el usuario.
*/
int menuGestionUsuarios(){
  printf("**********************\n");
  printf("*Gestion de Usuarios*\n");
  printf("**********************\n");
  printf("1. Agregar usuario.\n");
  // printf("2. Eliminar usuario.\n");
  printf("2. Ver usuarios.\n");
  printf("3. Volver.\n");
  printf("Ingrese una opcion: ");
  return seleccionOpcion();
}

/*
A. Top 3 de producciones (nombre) más prestadas. 
B. Top 3 de usuarios con más préstamos.
C. Top 5 de mes-año con mayor monto recaudado (según fecha de inicio
*/
/**
 * Mostrar menu de estadisticas.
 * 
 * Funcion que muestra el menu de estadisticas.
 * 
 * @return opcion ingresada por el usuario.
*/
int menuEstadisticas(){
  printf("**************\n");
  printf("*Estadísticas*\n");
  printf("**************\n");
  printf("1. Top 3 de producciones más prestadas.\n");
  printf("2. Top 3 de usuarios con más préstamos.\n");
  printf("3. Top 5 de mes-año con mayor monto recaudado.\n");
  printf("4. Volver.\n");
  printf("Ingrese una opcion: ");
  return seleccionOpcion();
}

#endif