#ifndef OPCIONESGENERALES_H
#define OPCIONESGENERALES_H


/**
 * @file opcionesGenerales.h
 * @brief Funciones para gestionar las opciones generales de la biblioteca (busqueda, prestamo, devolucion)
 * @version 1.0
 * @date 29/09/2023
 * @author @Andyporras,@miltonials
*/

/**
 * Prestamo en PDF
 * 
 * Esta función genera un PDF con los datos del préstamo
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @param prestamo Puntero al préstamo
 * @param usuario Puntero al usuario
 * @param libro Puntero al libro
 * @return void
*/
void prestamoApdf(Biblioteca *dirM_biblioteca, Prestamo *prestamo, Usuario *usuario, Libro *libro)
{
  char contenido[500]; // Ajusta el tamaño según tus necesidades
  // la ruta es dirM_biblioteca->rutaArchivos más la palabra comprobantes
  char *ruta = malloc(strlen(dirM_biblioteca->rutaArchivos) + strlen("comprobantes") + 1);
  strcpy(ruta, dirM_biblioteca->rutaArchivos);
  strcat(ruta, "comprobantes/");

  // Crear el contenido del PDF utilizando los datos del préstamo, usuario y libro
  snprintf(contenido, sizeof(contenido),
           "Detalles del Préstamo:\n\n"
           "ID de Préstamo: %d\n"
           "Cédula de Usuario: %s (%s)\n"
           "Título de Libro: %s\n"
           "Fecha de Inicio: %s\n"
           "Fecha de Fin: %s\n",
           prestamo->id, prestamo->cedulaUsuario, usuario->nombre,
           libro->titulo, prestamo->fechaInicio, prestamo->fechaFin);

  // Generar el nombre del archivo (puedes personalizarlo)
  char nombreArchivo[50];
  snprintf(nombreArchivo, sizeof(nombreArchivo), "prestamo_%d", prestamo->id);

  // Llamar a la función para crear el PDF
  printf(" %s", contenido);
  crearPDF(ruta, nombreArchivo, contenido);

  printf("PDF de préstamo creado: %s\n", nombreArchivo);
}

/**
 * Generar comprobante
 * 
 * Esta función genera un comprobante de préstamo y actualiza los archivos de prestamos, 
 * catalogo de libros y genera un PDF con los datos del préstamo
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @param usuario Puntero al usuario
 * @param libro Puntero al libro
 * @param fechaInicio Puntero a la fecha de inicio
 * @param fechaFinal Puntero a la fecha final
 * @return void
*/
void generarComprobante(Biblioteca *dirM_biblioteca, Usuario *usuario, Libro *libro, char *fechaInicio, char *fechaFinal)
{
  // validar que este disponible el libro
  Prestamo *prestamos = dirM_biblioteca->prestamos;
  Libro *libros = dirM_biblioteca->libros;
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  int cantidadPrestamos = dirM_biblioteca->cantidadPrestamos;
  int libroDisponible = libro->cantidad;
  int cedula = atoi(usuario->cedula);
  char *titulo;
  titulo = malloc(strlen(libro->titulo) + 1);
  strcpy(titulo, libro->titulo);

  if (libroDisponible > 0)
  {
    // generar comprobante
    Prestamo prestamo;
    prestamo.id = cantidadPrestamos + 1;
    prestamo.idLibro = libro->id;
    prestamo.cedulaUsuario = malloc(strlen(usuario->cedula) + 1);
    strcpy(prestamo.cedulaUsuario, usuario->cedula);
    prestamo.fechaInicio = malloc(strlen(fechaInicio) + 1);
    strcpy(prestamo.fechaInicio, fechaInicio);
    prestamo.fechaFin = malloc(strlen(fechaFinal) + 1);
    strcpy(prestamo.fechaFin, fechaFinal);
    prestamo.estado = 1;
    prestamo.fechaDevolucion = malloc(strlen("") + 1);
    strcpy(prestamo.fechaDevolucion, "");

    // prestamos = realloc(prestamos, sizeof(Prestamo) * (cantidadPrestamos + 1));
    prestamos[cantidadPrestamos] = prestamo;
    dirM_biblioteca->prestamos = prestamos;
    dirM_biblioteca->cantidadPrestamos++;

    libro->cantidad--;

    actualizarPrestamos(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
    cargarPrestamos(dirM_biblioteca);
    actualizarCatalogo(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
    prestamoApdf(dirM_biblioteca, &prestamo, usuario, libro);
    printf("El libro %s ha sido prestado al usuario %s\n", titulo, usuario->nombre);
  }
  else
  {
    printf("El libro no esta disponible\n");
  }
}

/**
 * Prestamo de ejemplar
 * 
 * Esta función realiza el prestamo de un ejemplar y genera un comprobante de prestamo
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @return void
*/
void prestamoEjemplar(Biblioteca *dirM_biblioteca)
{
  Usuario *usuarios = dirM_biblioteca->usuarios;
  Libro *libros = dirM_biblioteca->libros;
  int cantidadLibros = dirM_biblioteca->cantidadLibros;
  int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
  char cedula[20];
  int id = 0;
  // char titulo[50];
  char fechaInicio[20];
  char fechaFinal[20];

  printf("Ingrese la cedula del usuario: ");
  scanf(" %[^\n]s", cedula);
  // se pide el titulo del libro se acepta espacios
  printf("Ingrese el id del libro/ejemplar: ");
  scanf("%d", &id);
  printf("Ingrese la fecha de inicio: ");
  scanf("%s", fechaInicio);
  printf("Ingrese la fecha final: ");
  scanf("%s", fechaFinal);

  Libro *libro = buscarLibro_id(dirM_biblioteca, id);
  Usuario *usuario = existeUsuario(dirM_biblioteca, cedula);
  if (usuario != NULL)
  {
    if (libro != NULL)
    {
      if (formatoFecha(fechaInicio) == 1 || formatoFecha(fechaFinal) == 1)
      {
        // validar que la fecha de inicio sea menor a la fecha final
        if (validarRangoFechas(fechaInicio, fechaFinal) == 1)
        {

          generarComprobante(dirM_biblioteca, usuario, libro, fechaInicio, fechaFinal);
        }
        else
        {
          printf("La fecha dinal debe ser mayor a la fecha de inicio del préstamo.\n");
        }
      }
      else
      {
        printf("El formato de fecha es incorrecto.\n");
      }
    }
    else
    {
      printf("El libro no existe.\n");
    }
  }
  else
  {
    printf("El usuario no existe\n");
  }

  pausar("Presione enter para volver al menú...");
  limpiarPantalla();
}

/**
 * Existe prestamo
 * 
 * Esta función verifica si existe un prestamo en la biblioteca por medio de su id
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @param prestamo_id Identificador del prestamo
 * @return &prestamos[i] Puntero al prestamo si existe o NULL si no existe
*/
Prestamo *existePrestamo(Biblioteca *dirM_biblioteca, int prestamo_id)
{
  Prestamo *prestamos = dirM_biblioteca->prestamos;
  int cantidadPrestamos = dirM_biblioteca->cantidadPrestamos;
  int i;
  for (i = 0; i < cantidadPrestamos; i++)
  {
    if (prestamos[i].id == prestamo_id)
    {
      return &prestamos[i];
    }
  }
  return NULL;
}

/*
El sistema debe permitir devolver un ejemplar, para esto solicitará el identificador de préstamo y
la fecha de devolución. El sistema calculará el monto asociado al préstamo de la siguiente manera:

1 a 7 días: 150 (tarificación por día) y 75 (tarificación por día tardío)
8 a 15 días: 125 (tarificación por día) y 50 (tarificación por día tardío)
16 o más días: 100 (tarificación por día) y 25 (tarificación por día tardío)

formula: (dias * tarificacion) + (diasTardios * tarificacionTardia)
*/
/**
 * Devolucion de ejemplar
 * 
 * Esta función realiza la devolucion de un ejemplar y genera un comprobante de devolucion
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @return void
*/
void devolucionEjemplar(Biblioteca *dirM_biblioteca)
{
  Prestamo *prestamos = dirM_biblioteca->prestamos;
  int cantidadPrestamos = dirM_biblioteca->cantidadPrestamos;
  int idPrestamo;
  char fechaDevolucion[20];
  int dias;
  int diasTardios;
  int tarificacion;
  int tarificacionTardia;
  int monto;
  int i;

  printf("Ingrese el id del prestamo: ");
  scanf("%d", &idPrestamo);
  printf("Ingrese la fecha de devolucion: ");
  scanf("%s", fechaDevolucion);

  Prestamo *prestamo = existePrestamo(dirM_biblioteca, idPrestamo);

  if (prestamo != NULL)
  {
    if (prestamo->estado == 0)
    {
      printf("El prestamo ya fue devuelto\n");
    }
    else if (formatoFecha(fechaDevolucion) == 1)
    {
      Libro *libro = buscarLibro_id(dirM_biblioteca, prestamo->idLibro);
      dias = diferenciaDias(prestamo->fechaInicio, fechaDevolucion);
      diasTardios = diferenciaDias(fechaDevolucion, prestamo->fechaFin);
      if (dias < 0)
      {
        pausar("La fecha de devolución no puede ser menor a la fecha de inicio del préstamo.");
        limpiarPantalla();
        return;
      }
      if (diasTardios < 0)
      {
        diasTardios = 0;
      }
      if (dias <= 7)
      {
        tarificacion = 150;
        tarificacionTardia = 75;
      }
      else if (dias <= 15)
      {
        tarificacion = 125;
        tarificacionTardia = 50;
      }
      else
      {
        tarificacion = 100;
        tarificacionTardia = 25;
      }
      monto = (dias * tarificacion) + (diasTardios * tarificacionTardia);
      printf("El monto a pagar es: %d\n", monto);

      libro->cantidad++;
      prestamo->estado = 0;

      actualizarCatalogo(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
      actualizarPrestamos(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
    }
    else
    {
      printf("El formato de fecha es incorrecto\n");
    }
  }
  else
  {
    printf("El prestamo no existe\n");
  }

  pausar("Presione enter para volver al menú...");
  limpiarPantalla();
}

/**
 * Busqueda simple
 * 
 * Esta función realiza la busqueda simple de un ejemplar y muestra sus detalles
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @return void
*/
void busquedaSimple(Biblioteca *dirM_biblioteca)
{
  char textoUsuario[200];
  printf("Ingrese el texto a buscar: ");
  scanf(" %[^\n]s", textoUsuario);

  limpiarPantalla();

  for (int i = 0; i < dirM_biblioteca->cantidadLibros; i++)
  {
    Libro *ejemplar = &(dirM_biblioteca->libros[i]);

    // Verificar si el texto está contenido en nombre, autor o resumen
    if (strstr(ejemplar->titulo, textoUsuario) != NULL ||
        strstr(ejemplar->autor, textoUsuario) != NULL ||
        strstr(ejemplar->resumen, textoUsuario) != NULL)
    {

      // Mostrar los detalles del ejemplar
      printf("Identificador: %d\n", ejemplar->id);
      printf("Nombre: %s\n", ejemplar->titulo);
      printf("Resumen: %s\n", ejemplar->resumen);

      if (ejemplar->cantidad > 0)
      {
        printf("Estado: Disponible\n");
      }
      else
      {
        printf("Estado: No disponible\n");
      }

      printf("\n");
    }
  }
}

// Función auxiliar para verificar si un texto está contenido o coincide exactamente
/**
 * Contiene texto
 * 
 * Esta función verifica si un texto esta contenido o coincide exactamente con otro texto, dependiendo de la tecnica
 * 
 * @param campo Puntero al campo
 * @param texto Puntero al texto
 * @param tecnica Tecnica de busqueda
 * @return bool Si cumple o no los criterios
*/
bool contieneTexto(char *campo, char *texto, char tecnica)
{
  if (tecnica == 'c')
  {
    return (strstr(campo, texto) != NULL);
  }
  else if (tecnica == 'e')
  {
    return (strcmp(campo, texto) == 0);
  }
  return false;
}

/**
 * Solicitar tecnica de busqueda avanzada
 * 
 * Esta función solicita la tecnica de busqueda avanzada
 * 
 * @param texto Puntero al texto
 * @return char Tecnica de busqueda
*/
char solicitarTecnica_busquedaAvanzada(char *texto)
{
  if (strlen(texto) == 0)
  {
    return 'c';
  }
  
  char tecnica;
  printf("Ingrese técnica (c: contiene, e: exacta): ");
  scanf(" %c", &tecnica);
  return tecnica;
}

/**
 * Solicitar texto de busqueda avanzada
 * 
 * Esta función solicita el texto de busqueda avanzada
 * 
 * @param texto Puntero al texto
 * @param campo Puntero al campo
 * @return void
*/
void solicitarTexto_busquedaAvanzada(char *texto, const char *campo) {
    printf("Ingrese texto para buscar en %s (deje en blanco si no desea buscar en este campo): ", campo);
    
    int i = 0;
    char ch;

    // limpiar el stdin
    while (getchar() != '\n');

    while (i < 199 && (ch = getchar()) != EOF && ch != '\n') {
        texto[i] = ch;
        i++;
    }

    texto[i] = '\0';
}

/**
 * Solicitar operador de busqueda avanzada
 * 
 * Esta función solicita el operador de busqueda avanzada
 * 
 * @return char Operador de busqueda
*/
char solicitarOperador()
{
  char operador;
  printf("Ingrese operador entre campos (o: 'o', y: 'y'): ");
  scanf(" %c", &operador);
  return operador;
}

/**
 * Cumple criterios
 * 
 * Esta función verifica si cumple los criterios de busqueda avanzada
 * 
 * @param ejemplar Puntero al ejemplar
 * @param texto Puntero al texto
 * @param tecnica Tecnica de busqueda
 * @param campo Puntero al campo
 * @return bool Si cumple o no los criterios
*/
bool cumpleCriterios(Libro *ejemplar, char *texto, char tecnica, char *campo)
{
  return (strlen(texto) == 0 || contieneTexto(campo, texto, tecnica));
}

/**
 * Cumple operador
 * 
 * Esta función verifica si cumple el operador de busqueda avanzada
 * 
 * @param matches Puntero a los matches
 * @param operador Operador de busqueda
 * @return bool Si cumple o no los criterios
*/
bool cumpleOperador(bool *matches, char operador)
{
  if (operador == 'o')
  {
    return matches[0] || matches[1] || matches[2] || matches[3];
  }
  else if (operador == 'y')
  {
    return matches[0] && matches[1] && matches[2] && matches[3];
  }
  return false;
}

/**
 * Mostrar detalle
 * 
 * Esta función muestra el detalle de un ejemplar
 * 
 * @param ejemplar Puntero al ejemplar
 * @return void
*/
void mostrarDetalle(Libro *ejemplar)
{
  printf("Identificador: %d\n", ejemplar->id);
  printf("Nombre: %s\n", ejemplar->titulo);
  printf("Resumen: %s\n", ejemplar->resumen);
  printf("\n");
}

/**
 * Busqueda avanzada
 * 
 * Esta función realiza la busqueda avanzada de un ejemplar y muestra sus detalles
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @return void
*/
void busquedaAvanzada(Biblioteca *dirM_biblioteca)
{
  // Solicitar detalles de búsqueda
  char textoNombre[200];
  char textoAutor[200];
  char textoGenero[200];
  char textoResumen[200];

  char operador;

  solicitarTexto_busquedaAvanzada(textoNombre, "el nombre");
  char tecnicaNombre = solicitarTecnica_busquedaAvanzada(textoNombre);
  solicitarTexto_busquedaAvanzada(textoAutor, "el autor");
  char tecnicaAutor = solicitarTecnica_busquedaAvanzada(textoAutor);
  solicitarTexto_busquedaAvanzada(textoGenero, "el género");
  char tecnicaGenero = solicitarTecnica_busquedaAvanzada(textoGenero);
  solicitarTexto_busquedaAvanzada(textoResumen, "el resumen");
  char tecnicaResumen = solicitarTecnica_busquedaAvanzada(textoResumen);
  operador = solicitarOperador();

  // Iterar a través de cada ejemplar en el array de libros
  for (int i = 0; i < dirM_biblioteca->cantidadLibros; i++)
  {
    Libro *ejemplar = &(dirM_biblioteca->libros[i]);
    bool matches[4];

    matches[0] = cumpleCriterios(ejemplar, textoNombre, tecnicaNombre, ejemplar->titulo);
    matches[1] = cumpleCriterios(ejemplar, textoAutor, tecnicaAutor, ejemplar->autor);
    matches[2] = cumpleCriterios(ejemplar, textoGenero, tecnicaGenero, ejemplar->genero);
    matches[3] = cumpleCriterios(ejemplar, textoResumen, tecnicaResumen, ejemplar->resumen);

    if (cumpleOperador(matches, operador))
    {
      printf("\n");
      mostrarDetalle(ejemplar);
    }
  }
}

/**
 * Menu opciones generales
 * 
 * Esta función muestra el menu de opciones generales y captura la opcion seleccionada
 * 
 * @param dirM_biblioteca Puntero a la biblioteca
 * @return void
*/
void opcionesGenerales(Biblioteca *dirM_biblioteca)
{
  int opcion = menuOpcionesGenerales();
  while (opcion != 5)
  {
    switch (opcion)
    {
    case 1:
      printf("Busqueda simple.\n");
      busquedaSimple(dirM_biblioteca);

      break;
    case 2:
      printf("Busqueda avanzada.\n");
      busquedaAvanzada(dirM_biblioteca);
      pausar("Presione enter para volver al menú...");
      limpiarPantalla();
      break;
    case 3:
      printf("Prestamo de ejemplar.\n");
      prestamoEjemplar(dirM_biblioteca);
      pausar("Presione enter para volver al menú...");
      limpiarPantalla();
      break;
    case 4:
      printf("Devolucion de ejemplar.\n");
      devolucionEjemplar(dirM_biblioteca);
      break;
    case 5:
      printf("Volver.\n");
      break;
    default:
      printf("Opcion invalida.\n");
      break;
    }
    opcion = menuOpcionesGenerales();
  }
}

#endif // OPCIONESGENERALES_H