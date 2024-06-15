#include <stdio.h>
#include "read_json.h"
#include "operaciones_indv.h"

int main() {
    int opcion;

    do {
        // Imprimir el menú
        printf("Selecciona una opción:\n"); 
        printf("1. Carga masiva de usuarios\n");
        printf("2. Operaciones individuales\n");
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
                printf("MENU OPERACIONES INDIVIDUALES\n"); 
                printf("1. Deposito\n");
                printf("2. Retiro\n");
                printf("3. Transacciòn\n");
                printf("4. Consultar cuenta\n");
                printf("\n");
                printf("Ingresa el número de opción: ");
                scanf("%d", &opcion);
                switch (opcion) {
                    case 1:
                        printf("-------Deposito-------\n");
                        printf("Ingresa el número de cuenta: ");
                        int no_cuenta;
                        scanf("%d", &no_cuenta);
                        printf("Ingresa el monto: ");
                        float monto;
                        scanf("%f", &monto);
                        depositos(no_cuenta, monto);
                        break;
                    case 2:
                        printf("-------Retiro-------\n");
                        printf("Ingresa el número de cuenta: ");
                        int no_cuentaa;
                        scanf("%d", &no_cuentaa);
                        printf("Ingresa el monto: ");
                        float montoo;
                        scanf("%f", &montoo);
                        retiros(no_cuentaa, montoo);
                        break;
                    case 3:
                        printf("-------Transacciòn-------\n");
                        printf("Ingresa el número de cuenta origen: ");
                        int no_cuenta_origen;
                        scanf("%d", &no_cuenta_origen);
                        printf("Ingresa el número de cuenta destino: ");
                        int no_cuenta_destino;
                        scanf("%d", &no_cuenta_destino);
                        printf("Ingresa el monto: ");
                        float montooo;
                        scanf("%f", &montooo);
                        transacciones(no_cuenta_origen, no_cuenta_destino, montooo);
                        break;
                    case 4:
                        printf("-------Consultar cuenta-------\n");
                        printf("Ingresa el número de cuenta: ");
                        int no_cuentaaaa;
                        scanf("%d", &no_cuentaaaa);
                        consultar_cuenta(no_cuentaaaa);
                        break;
                    default:
                        printf("Opción inválida. Por favor, selecciona una opción válida.\n");
                        break;
                }

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

//gcc main.c read_json.c operaciones_indv.c cJSON.c -o output
//./output
