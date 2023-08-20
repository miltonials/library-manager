#include <stdio.h>
#include <json-c/json.h>

typedef struct {
  int id;
  char titulo[50];
  char autor[50];
  int anio;
  char genero[50];
  char resumen[500];
  int cantidad;
} Libro;



int main() {
    FILE *fp;
    char buffer[5000];
    struct json_object *parsed_json;
    size_t n, i;

    fp = fopen("catalogo.json", "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    fread(buffer, sizeof(buffer), 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);
    if (parsed_json == NULL) {
        printf("Error al analizar el JSON.\n");
        return 1;
    }

    n = json_object_array_length(parsed_json);
    Libro *libros = malloc(sizeof(Libro) * n);
    for (i = 0; i < n; i++) {
        struct json_object *obj = json_object_array_get_idx(parsed_json, i);

        struct json_object *id_obj, *titulo_obj, *autor_obj, *anio_obj, *genero_obj, *resumen_obj, *cantidad_obj;

        json_object_object_get_ex(obj, "id", &id_obj);
        json_object_object_get_ex(obj, "titulo", &titulo_obj);
        json_object_object_get_ex(obj, "autor", &autor_obj);
        json_object_object_get_ex(obj, "anio", &anio_obj);
        json_object_object_get_ex(obj, "género", &genero_obj);
        json_object_object_get_ex(obj, "resumen", &resumen_obj);
        json_object_object_get_ex(obj, "cantidad", &cantidad_obj);

        int id = json_object_get_int(id_obj);
        const char *titulo = json_object_get_string(titulo_obj);
        const char *autor = json_object_get_string(autor_obj);
        int anio = json_object_get_int(anio_obj);
        const char *genero = json_object_get_string(genero_obj);
        const char *resumen = json_object_get_string(resumen_obj);
        int cantidad = json_object_get_int(cantidad_obj);

        libros[i].id = id;
        strcpy(libros[i].titulo, titulo);
        strcpy(libros[i].autor, autor);
        libros[i].anio = anio;
        strcpy(libros[i].genero, genero);
        strcpy(libros[i].resumen, resumen);
        libros[i].cantidad = cantidad;
        
    }
    // mostrar el arreglo de libros
    for (i = 0; i < n; i++) {
        printf("Libro %d:\n", i + 1);
        printf("id: %d\n", libros[i].id);
        printf("titulo: %s\n", libros[i].titulo);
        printf("autor: %s\n", libros[i].autor);
        printf("anio: %d\n", libros[i].anio);
        printf("genero: %s\n", libros[i].genero);
        printf("resumen: %s\n", libros[i].resumen);
        printf("cantidad: %d\n", libros[i].cantidad);
    }



    json_object_put(parsed_json); // Liberar memoria del objeto JSON

    return 0;
}
