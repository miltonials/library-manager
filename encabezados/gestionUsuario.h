#ifndef GESTIONUSUARIO_H
#define GESTIONUSUARIO_H

Usuario *cargarUsuarios(char *rutaArchivo){
    FILE *fp;
    char buffer[5000];
    struct json_object *parsed_json;
    size_t n, i;

    fp = fopen(rutaArchivo, "r");
    fread(buffer, sizeof(buffer), 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);
    if (parsed_json == NULL){
        printf("Error al parsear el archivo\n");
        return NULL;
    }

    n = json_object_array_length(parsed_json);
    Usuario *usuarios = malloc(sizeof(Usuario) * n);
    for(i = 0; i < n; i++){
        struct json_object *obj = json_object_array_get_idx(parsed_json, i);

        struct json_object *cedula_obj, *nombre_obj, *direccion_obj;

        json_object_object_get_ex(obj, "cedula", &cedula_obj);
        json_object_object_get_ex(obj, "nombre", &nombre_obj);
        json_object_object_get_ex(obj, "direccion", &direccion_obj);

        int cedula = json_object_get_int(cedula_obj);
        const char *nombre = json_object_get_string(nombre_obj);
        const char *direccion = json_object_get_string(direccion_obj);

        usuarios[i].cedula = cedula;
        strcpy(usuarios[i].nombre, nombre);
        strcpy(usuarios[i].direccion, direccion);

    }

    printf("Usuarios cargados\n");
    for (int i = 0; i < n; i++){
        printf("Usuario %d\n", i + 1);
        printf("Cedula: %d\n", usuarios[i].cedula);
        printf("Nombre: %s\n", usuarios[i].nombre);
        printf("Direccion: %s\n", usuarios[i].direccion);
        printf("\n");
    }

    json_object_put(parsed_json);
    return usuarios;
}

int contarUsuarios(Usuario *usuarios){
    int cantidadUsuarios = 0;
    while (usuarios[cantidadUsuarios].cedula != 0){
        cantidadUsuarios++;
    }
    return cantidadUsuarios;
}
void imprimirUsuarios(Usuario *usuarios, int cantidadUsuarios){
    for (int i = 0; i < cantidadUsuarios; i++){
        printf("Usuario %d\n", i + 1);
        printf("Cedula: %d\n", usuarios[i].cedula);
        printf("Nombre: %s\n", usuarios[i].nombre);
        printf("Direccion: %s\n", usuarios[i].direccion);
        printf("\n");
    }

}

void verUsuarios(){
    printf("Ver usuarios\n");
    Usuario *usuarios = cargarUsuarios("data/usuarios.json");

    int cantidadUsuarios = contarUsuarios(usuarios);
    imprimirUsuarios(usuarios, cantidadUsuarios);
    printf("Cantidad total de usuarios %d\n", cantidadUsuarios);

}

void opcionGestionUsuarios(){
    int opcion = menuGestionUsuarios();
    while (opcion != 4){
        switch (opcion){
            case 1:
                // registrarUsuario();
                break;
            case 2:
                // eliminarUsuario();
                break;
            case 3:
                verUsuarios();
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