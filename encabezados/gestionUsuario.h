#ifndef GESTIONUSUARIO_H
#define GESTIONUSUARIO_H

/**
 * @file gestionUsuario.h
 * @brief Funciones para gestionar los usuarios de la biblioteca
 * @version 1.0
 * @date 29/09/2023
 * @author @Andyporras
*/

/**
 * Ver usuarios.
 * 
 * Funcion que muestra los usuarios registrados en la biblioteca.
 * 
 * @param dirM_biblioteca Puntero a la biblioteca.
 * @return Ninguna.
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

/**
 * Solicitar datos de usuario.
 * 
 * Funcion que solicita los datos de un usuario al usuario.
 * 
 * @return Una estructura de tipo Usuario con los datos ingresados.
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

/**
 * Existe usuario.
 * 
 * Funcion que verifica si un usuario ya existe en la biblioteca. Si existe, retorna un puntero a la estructura del usuario.
 * 
 * @param dirM_biblioteca Puntero a la biblioteca.
 * @param cedula Cedula del usuario a buscar.
 * @return Puntero a la estructura del usuario si existe, NULL si no existe.
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

/**
 * Registrar usuario.
 * 
 * Funcion que registra un usuario en la biblioteca.
 * 
 * @param dirM_biblioteca Puntero a la biblioteca.
 * @return Ninguna.
*/
void registrarUsuario(Biblioteca *dirM_biblioteca){
    Usuario usuarioNuevo = solicitarDatosUsuario();
    Usuario* busquedaUsuario = existeUsuario(dirM_biblioteca, usuarioNuevo.cedula);
    if (busquedaUsuario == NULL) {
        //mostrar mensaje de que el usuario ya existe.
        printf("\n× Ya existe un usuario con la cedula %s.\n", usuarioNuevo.cedula);
        return;
    }
    
    int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
    dirM_biblioteca->usuarios = realloc(dirM_biblioteca->usuarios, sizeof(Usuario) * (cantidadUsuarios + 1));
    dirM_biblioteca->usuarios[cantidadUsuarios] = usuarioNuevo;
    dirM_biblioteca->cantidadUsuarios++;
    
    actualizarUsuarios(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
}

/**
 * Opciones gestion usuarios.
 * 
 * Funcion que muestra el menu de opciones de gestion de usuarios.
 * 
 * @param dirM_biblioteca Puntero a la biblioteca.
 * @return ninguna.
*/
void opcionGestionUsuarios(Biblioteca *dirM_biblioteca){
    int opcion = menuGestionUsuarios();
    while (opcion != 3){
        switch (opcion){
            case 1:
                registrarUsuario(dirM_biblioteca);
                pausar("Presione enter para volver al menú...");
                limpiarPantalla();
                break;
            case 2:
                verUsuarios(dirM_biblioteca);
                break;
            case 3:
                break;
            default:
                printf("Opcion invalida.\n");
                break; 
        }
        opcion = menuGestionUsuarios();
    }
}



#endif // GESTIONUSUARIO_H