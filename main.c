#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "./encabezados/menus.h"
#include "./encabezados/estructuras.h"
#include "./encabezados/opcionesGenerales.h"
#include "./encabezados/opcionesOperativas.h"

int main()
{
  int opcion = -1;

  while (opcion != 3)
  {
    opcion = menuPrincipal();
    
    switch (opcion)
    {
      case 1:
        opcionesOperativas();
        break;
      case 2:
        opcionesGenerales();
        break;
      case 3:
        printf("👋 Gracias por usar el sistema de gestion de biblioteca.\n");
        break;
      default:
        printf("Opcion invalida.\n");
        break;
    }
  }

  return 0;
}
