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
    json_object_put(parsed_json);
    return usuarios;
}

int contarUsuarios(char *rutaArchivo){
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
    return n;
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

    int cantidadUsuarios = contarUsuarios("data/usuarios.json");
    imprimirUsuarios(usuarios, cantidadUsuarios);
    printf("Cantidad total de usuarios %d\n", cantidadUsuarios);

}
Usuario solicitarDastosUsuario(){
    Usuario UsuarioNuevo;
    printf("Ingrese la cedula del usuario: ");
    scanf("%d", &UsuarioNuevo.cedula);
    printf("Ingrese el nombre del usuario: ");
    scanf("%s", UsuarioNuevo.nombre);
    printf("Ingrese la direccion del usuario: ");
    scanf("%s", UsuarioNuevo.direccion);
    return UsuarioNuevo;
}

void registrarUsuario(){
    Usuario usuarioNuevo = solicitarDastosUsuario();
    Usuario *usuarios = cargarUsuarios("data/usuarios.json");
    //se guarda el usuario nuevo en el archivo json,
    // usando la libreria json-c
    FILE *fp;
    char buffer[5000];
    struct json_object *parsed_json;
    struct json_object *json_usuarios;
    struct json_object *json_usuario;
    struct json_object *json_cedula;
    struct json_object *json_nombre;
    struct json_object *json_direccion;
    size_t n;

    fp = fopen("data/usuarios.json", "r");
    fread(buffer, sizeof(buffer), 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);
    if(parsed_json == NULL){
        printf("Error al parsear el archivo\n");
        return;
    }

    n = json_object_array_length(parsed_json);
    json_usuarios = json_object_new_array();
    for (int i = 0; i < n; i++){
        json_usuario = json_object_new_object();

        json_cedula = json_object_new_int(usuarios[i].cedula);
        json_object_object_add(json_usuario, "cedula", json_cedula);

        json_nombre = json_object_new_string(usuarios[i].nombre);
        json_object_object_add(json_usuario, "nombre", json_nombre);

        json_direccion = json_object_new_string(usuarios[i].direccion);
        json_object_object_add(json_usuario, "direccion", json_direccion);

        json_object_array_add(json_usuarios, json_usuario);
    }
    json_usuario = json_object_new_object();

    json_cedula = json_object_new_int(usuarioNuevo.cedula);
    json_object_object_add(json_usuario, "cedula", json_cedula);

    json_nombre = json_object_new_string(usuarioNuevo.nombre);
    json_object_object_add(json_usuario, "nombre", json_nombre);

    json_direccion = json_object_new_string(usuarioNuevo.direccion);
    json_object_object_add(json_usuario, "direccion", json_direccion);

    json_object_array_add(json_usuarios, json_usuario);

    fp = fopen("data/usuarios.json", "w");
    json_object_to_file_ext("data/usuarios.json", json_usuarios, JSON_C_TO_STRING_PRETTY);
    fclose(fp);
    json_object_put(parsed_json);
    printf("Usuario registrado con exito\n");
    free(usuarios);

}

void opcionGestionUsuarios(){
    int opcion = menuGestionUsuarios();
    while (opcion != 4){
        switch (opcion){
            case 1:
                registrarUsuario();
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