#ifndef ARCHIVOS_H
#define ARCHIVOS_H

char* leerArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tamaño = ftell(archivo);
    rewind(archivo);

    char* contenido = (char*)malloc(tamaño + 1);
    if (contenido == NULL) {
        perror("Error al asignar memoria");
        fclose(archivo);
        return NULL;
    }

    size_t bytesLeidos = fread(contenido, 1, tamaño, archivo);
    if (bytesLeidos != tamaño) {
        perror("Error al leer el archivo");
        fclose(archivo);
        free(contenido);
        return NULL;
    }

    contenido[tamaño] = '\0';
    fclose(archivo);
    return contenido;
}

int escribirArchivo(char* ruta, char* contenido){
    FILE* archivo = fopen(ruta, "w");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    size_t bytesEscritos = fwrite(contenido, 1, strlen(contenido), archivo);
    if (bytesEscritos != strlen(contenido)) {
        perror("Error al escribir el archivo");
        fclose(archivo);
        return -2;
    }

    fclose(archivo);
    return 1;
}


#endif // ARCHIVOS_H