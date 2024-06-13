#include <stdio.h>
#include "read_json.h"

int main() {
    int opcion;

    do {
        // Imprimir el menú
        printf("Selecciona una opción:\n"); 
        printf("1. Carga masiva de usuarios\n");
        printf("2. Opcion 2\n");
        printf("3. Opcion 3\n");
        printf("4. Salir\n");

        // Solicitar la opción al usuario
        printf("Ingresa el número de opción: ");
        scanf("%d", &opcion);

        // Realizar la acción según la opción seleccionada
        switch (opcion) {
            case 1:
                char filename[100];
                printf("Ingresa la ruta del archivo: ");
                scanf("%s", filename);
                
                
                read_json_file(filename); //  leemos el archivo json
                break;
            case 2:
                printf("Seleccionaste la Opcion 2\n");
                break;
            case 3:
                printf("Seleccionaste la Opcion 3\n");
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción inválida. Por favor, selecciona una opción válida.\n");
                break;
        }

        printf("\n");
    } while (opcion != 4);

    return 0;
}

// Compilar:  gcc main.c read_json.c cJSON.c -o output
// Ejecutar: ./output
