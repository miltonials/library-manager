#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h> // Para utilizar el tipo de dato bool
#include <json-c/json.h> //sudo apt-get install libjson-c-dev
#include <hpdf.h> // sudo apt-get install libharu-dev,  como se usa libreria externa, se debe compilar con -lhpdf

#include "./encabezados/archivos.h"
#include "./encabezados/tiempo.h"
#include "./encabezados/menus.h"
#include "./encabezados/estructuras.h"
#include "./encabezados/biblioteca/cargarDatos.h"
#include "./encabezados/biblioteca/actualizarDatos.h"
#include "./encabezados/gestionUsuario.h"
#include "./encabezados/opcionesOperativas.h"
#include "./encabezados/opcionesGenerales.h"

int main(){
  int opcion = 0;
  Biblioteca biblioteca;
  cargarBiblioteca(&biblioteca, "./data/");
  while (opcion != 4)
  {
    opcion = menuPrincipal();
    
    switch (opcion)
    {
      case 1:
        opcionesOperativas(&biblioteca);
        break;
      case 2:
        opcionesGenerales(&biblioteca);
        break;
      case 3:
        resumenBiblioteca(&biblioteca);
        pausar("Presione enter para volver...");
        limpiarPantalla();
        break;
      case 4:
        printf("👋 Gracias por usar el sistema de gestion de biblioteca.\n");
        break;
      default:
        printf("Opcion invalida.\n");
        break;
    }
  }
  return 0;
}
