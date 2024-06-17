#include <stdio.h>
#include "read_json.h"
#include "operaciones_indv.h"
#include <stdlib.h>
#include "cJSON.h"
#include <string.h>

//Funcion para transacciones 
struct data_struct3 {
    int operacion;
    int cuenta1;
    int cuenta2;
    float monto;
};
struct data_struct3 transaction[300];
int transacciones_size = 0;

struct data_struct2 {
    int linea;
    char error [30];
};
struct data_struct2 errores2[300];
int errores_size = 0;

void read_json_file2(char* filename){

    // Open the JSON file for reading
    FILE *file = fopen(filename, "r");
    if(!file){
        perror("Open File");
        return;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the entire file into a buffer
    char *buffer = (char*)malloc(filesize + 1);
    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0';

    // Close the file
    fclose(file);

    // Parse the JSON data
    cJSON *json = cJSON_Parse(buffer);
    if(json == NULL){
        perror("Parse JSON");
        return;
    }

    // Check if it's a valid JSON array
    if(!cJSON_IsArray(json)){
        perror("JSON is not Array");
        return;
    }

    // Iterate through the array

    // int array_size = cJSON_GetArraySize(json);
    // for(int i = 0; i< array_size; i++){
    //     cJSON *item = cJSON_GetArrayItem(json, i);
    //     if(cJSON_IsObject(item)){
    //         cJSON *id = cJSON_GetObjectItem(item, "id");
    //         cJSON *firstname = cJSON_GetObjectItem(item, "first_name");
    //         cJSON *lastname = cJSON_GetObjectItem(item, "last_name");
    //         cJSON *email = cJSON_GetObjectItem(item, "email");

    //         if(cJSON_IsNumber(id) && cJSON_IsString(firstname) 
    //             && cJSON_IsString(firstname) && cJSON_IsString(firstname)){

    //                 printf("ID: %d - Name: %s %s - Email: %s\n", id->valueint, 
    //                     firstname->valuestring, lastname->valuestring, email->valuestring);
    //             }

    //     }
    // }

    int n = 0;
    int n2 = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json){
        if(cJSON_IsObject(item)){
            cJSON *operacion = cJSON_GetObjectItem(item, "operacion");
            cJSON *cuenta1 = cJSON_GetObjectItem(item, "cuenta1");
            cJSON *cuenta2 = cJSON_GetObjectItem(item, "cuenta2");
            cJSON *monto = cJSON_GetObjectItem(item, "monto");

            if (cuenta1->valueint==cuenta2->valueint){
                //printf("Cuenta origen y destino iguales\n");
                errores2[n2].linea = n;
                strcpy(errores2[n2].error, "Cuenta origen y destino iguales");
                n2++;
            }else if (cuenta1->valueint <= 0 || cuenta2->valueint <= 0){
                //printf("Cuenta origen o destino negativa\n");
                errores2[n2].linea = n;
                strcpy(errores2[n2].error, "Cuenta origen o destino negativa");
                n2++;
            }else if (monto->valuedouble < 0){
                //printf("Monto negativo\n");
                errores2[n2].linea = n;
                strcpy(errores2[n2].error, "Monto negativo");
                n2++;
            } else if(cJSON_IsNumber(cuenta1) && cJSON_IsNumber(cuenta2) && cuenta1->valueint != cuenta2->valueint && cuenta1->valueint > 0 && cuenta2->valueint > 0 && monto->valuedouble > 0){

                    transaction[n].operacion = operacion->valueint;
                    transaction[n].cuenta1 = cuenta1->valueint;
                    transaction[n].cuenta2 = cuenta2->valueint;
                    transaction[n].monto = monto->valuedouble;
                    n++;
            }else {
                errores2[n2].linea = n;
                strcpy(errores2[n2].error, "Error desconocido");
                n2++;
            }


        }
    }
    transacciones_size = n;
    errores_size = n2;

    // Clean up
    cJSON_Delete(json);
    free(buffer);
}



int main()
{
    int opcion;

    do
    {
        // Imprimir el menú
        printf("Selecciona una opción:\n");
        printf("1. Carga masiva de usuarios\n");
        printf("2. Operaciones individuales\n");
        printf("3. Carga masiva de transacciones\n");
        printf("4. Reporte Estado de cuenta\n");
        
        printf("5. Salir\n");

        // Solicitar la opción al usuario
        printf("Ingresa el número de opción: ");
        scanf("%d", &opcion);

        // Realizar la acción según la opción seleccionada
        switch (opcion)
        {
        case 1:
            char filename[100];
            printf("Ingresa la ruta del archivo: ");
            scanf("%s", filename);

            read_json_file(filename); //  leemos el archivo json
            break;
        case 2:
            int subopcion;
            do
            {
                printf("MENU OPERACIONES INDIVIDUALES\n");
                printf("1. Deposito\n");
                printf("2. Retiro\n");
                printf("3. Transacciòn\n");
                printf("4. Consultar cuenta\n");
                printf("5. Volver al menu principal\n");
                printf("\n");
                printf("Ingresa el número de opción: ");
                scanf("%d", &subopcion);
                switch (subopcion)
                {
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
                case 5:
                    printf("volviendo al menu principal...\n");
                    break;
                default:
                    printf("Opción inválida. Por favor, selecciona una opción válida.\n");
                    break;
                }
                printf("\n");
            } while (subopcion != 5);

            break;

        case 3:
            char filename2[100];
            printf("\nIngresa la ruta del archivo: ");
            scanf("%s", filename2);
            printf("Cargando transacciones...\n");
            read_json_file2(filename2);
            break;
        case 4:

            printf("Reporte Estado de cuenta realizado con exito\n");
            reporte_estado_cuenta();
            break;
        case 5:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opción inválida. Por favor, selecciona una opción válida.\n");
            break;
        }

        printf("\n");
    } while (opcion != 5);

    return 0;
}

// Compilar:  gcc main.c read_json.c operaciones_indv.c cJSON.c -o output
// Ejecutar: ./output
