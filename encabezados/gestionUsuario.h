#ifndef GESTIONUSUARIO_H
#define GESTIONUSUARIO_H

/*
Funcion que muestra los usuarios registrados en la biblioteca
Entrada: Puntero a la biblioteca
Salida: Ninguna
*/
void verUsuarios(Biblioteca *dirM_biblioteca) {
    int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
    printf("Cantidad de usuarios: %d\n", cantidadUsuarios);
    for (int i = 0; i < cantidadUsuarios; i++) {
        printf("Usuario %d\n", i + 1);
        printf("Cedula: %s\n", dirM_biblioteca->usuarios[i].cedula);
        printf("Nombre: %s\n", dirM_biblioteca->usuarios[i].nombre);
        printf("Direccion: %s\n\n", dirM_biblioteca->usuarios[i].direccion);
    }

    pausar("Presione enter para volver al menú...");
    limpiarPantalla();
}

/*
Funcion que solicita los datos de un usuario
Entrada: Ninguna
Salida: Estructura de tipo Usuario
*/
Usuario solicitarDatosUsuario() {
    Usuario UsuarioNuevo;
    char cedula[20];
    char nombre[100];
    char direccion[100]; 

    printf("Ingrese la cedula del usuario: ");
    scanf(" %[^\n]s", cedula);
    printf("Ingrese el nombre del usuario: ");
    scanf(" %[^\n]s", nombre);
    printf("Ingrese la direccion del usuario: ");
    scanf("  %[^\n]s", direccion);

    UsuarioNuevo.cedula = malloc(strlen(cedula) + 1);
    strcpy(UsuarioNuevo.cedula, cedula);
    UsuarioNuevo.nombre = malloc(strlen(nombre) + 1);
    strcpy(UsuarioNuevo.nombre, nombre);
    UsuarioNuevo.direccion = malloc(strlen(direccion) + 1);
    strcpy(UsuarioNuevo.direccion, direccion);

    return UsuarioNuevo;
}

/*
Funcion que verifica si un usuario existe en la biblioteca
Entrada: Puntero a la biblioteca y cedula del usuario
Salida: Puntero al usuario si existe, NULL si no existe
*/
Usuario *existeUsuario(Biblioteca *dirM_biblioteca, char *cedula) {
    int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
    for (int i = 0; i < cantidadUsuarios; i++) {
        if (strcmp(dirM_biblioteca->usuarios[i].cedula, cedula) == 0) {
            return &dirM_biblioteca->usuarios[i];
        }
    }
    
    return NULL;
}

/*
Funcion que registra un usuario en la biblioteca
Entrada: Puntero a la biblioteca
Salida: Ninguna
*/
void registrarUsuario(Biblioteca *dirM_biblioteca){
    Usuario usuarioNuevo = solicitarDatosUsuario();

    if (existeUsuario(dirM_biblioteca, usuarioNuevo.cedula)) {
        return;
    }
    
    int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
    dirM_biblioteca->usuarios = realloc(dirM_biblioteca->usuarios, sizeof(Usuario) * (cantidadUsuarios + 1));
    dirM_biblioteca->usuarios[cantidadUsuarios] = usuarioNuevo;
    dirM_biblioteca->cantidadUsuarios++;
    
    actualizarUsuarios(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
}

/*
Funcion que muestra el menu de gestion de usuarios y captura la opcion seleccionada
Entrada: Ninguna
Salida: ninguna
*/
void opcionGestionUsuarios(Biblioteca *dirM_biblioteca){
    int opcion = menuGestionUsuarios();
    while (opcion != 4){
        switch (opcion){
            case 1:
                registrarUsuario(dirM_biblioteca);
                pausar("Presione enter para volver al menú...");
                limpiarPantalla();
                break;
            case 2:
                // eliminarUsuario();
                break;
            case 3:
                verUsuarios(dirM_biblioteca);
                break;
            case 4:
               //volver();
                break;
            default:
                printf("Opcion invalida.\n");
                break; 
        }
        opcion = menuGestionUsuarios();
    }
}



#endif // GESTIONUSUARIO_H