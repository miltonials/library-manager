#ifndef ARCHIVOS_H
#define ARCHIVOS_H

char* leerArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    rewind(archivo);

    char* contenido = (char*)malloc(tamano + 1);
    if (contenido == NULL) {
        perror("Error al asignar memoria");
        fclose(archivo);
        return NULL;
    }

    size_t bytesLeidos = fread(contenido, 1, tamano, archivo);
    if (bytesLeidos != tamano) {
        perror("Error al leer el archivo");
        fclose(archivo);
        free(contenido);
        return NULL;
    }

    contenido[tamano] = '\0';
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

void crearPDF(const char *ruta, const char *nombreArchivo, const char *contenido) {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Error al inicializar el PDF\n");
        return;
    }

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Page_BeginText(page);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);

    // HPDF_Page_TextOut(page, 100, 100, contenido);
    // que ocupe toda el area de la pagina y que imprima saltos de linea que tenga el contenido
    HPDF_Page_TextRect(page, 100, 600, 600, 100, contenido, HPDF_TALIGN_LEFT, NULL);

    HPDF_Page_EndText(page);

    char rutaCompleta[200];
    snprintf(rutaCompleta, sizeof(rutaCompleta), "%s/%s.pdf", ruta, nombreArchivo);

    HPDF_SaveToFile(pdf, rutaCompleta);
    HPDF_Free(pdf);
}

#endif // ARCHIVOS_H