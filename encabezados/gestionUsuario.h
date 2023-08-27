#ifndef GESTIONUSUARIO_H
#define GESTIONUSUARIO_H

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

int existeUsuario(Biblioteca *dirM_biblioteca, Usuario usuarioNuevo) {
    int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
    for (int i = 0; i < cantidadUsuarios; i++) {
        if (strcmp(dirM_biblioteca->usuarios[i].cedula, usuarioNuevo.cedula) == 0) {
            printf("El usuario %s ya está registrado con el número de cédula %s.\n", dirM_biblioteca->usuarios[i].nombre, dirM_biblioteca->usuarios[i].cedula);
            return 1;
        }
    }
    return 0;
}

void registrarUsuario(Biblioteca *dirM_biblioteca){
    Usuario usuarioNuevo = solicitarDatosUsuario();

    if (existeUsuario(dirM_biblioteca, usuarioNuevo)) {
        return;
    }
    
    int cantidadUsuarios = dirM_biblioteca->cantidadUsuarios;
    dirM_biblioteca->usuarios = realloc(dirM_biblioteca->usuarios, sizeof(Usuario) * (cantidadUsuarios + 1));
    dirM_biblioteca->usuarios[cantidadUsuarios] = usuarioNuevo;
    dirM_biblioteca->cantidadUsuarios++;
    
    actualizarUsuarios(dirM_biblioteca, dirM_biblioteca->rutaArchivos);
}

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