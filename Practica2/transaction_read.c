#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <string.h>

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
struct data_struct2 errores[300];
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
                printf("Cuenta origen y destino iguales\n");
                errores[n2].linea = n;
                strcpy(errores[n2].error, "Cuenta origen y destino iguales");
                n2++;
            }else if (cuenta1->valueint <= 0 || cuenta2->valueint <= 0){
                printf("Cuenta origen o destino negativa\n");
                errores[n2].linea = n;
                strcpy(errores[n2].error, "Cuenta origen o destino negativa");
                n2++;
            }else if (monto->valuedouble < 0){
                printf("Monto negativo\n");
                errores[n2].linea = n;
                strcpy(errores[n2].error, "Monto negativo");
                n2++;
            } else if(cJSON_IsNumber(cuenta1) && cJSON_IsNumber(cuenta2) && cuenta1->valueint != cuenta2->valueint && cuenta1->valueint > 0 && cuenta2->valueint > 0 && monto->valuedouble > 0){

                    transaction[n].operacion = operacion->valueint;
                    transaction[n].cuenta1 = cuenta1->valueint;
                    transaction[n].cuenta2 = cuenta2->valueint;
                    transaction[n].monto = monto->valuedouble;
                    n++;
            }else {
                errores[n2].linea = n;
                strcpy(errores[n2].error, "Error desconocido");
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

int main(){
    read_json_file2("transacciones.json");
    for(int i = 0; i < transacciones_size; i++){
        printf("Operacion: %d - Cuenta1: %d - Cuenta2: %d - Monto: %.2f\n", transaction[i].operacion, transaction[i].cuenta1, transaction[i].cuenta2, transaction[i].monto);
    }

    for(int i = 0; i < errores_size; i++){
        printf("Linea: %d - Error: %s\n", errores[i].linea, errores[i].error);
    }
    printf("transaction size: %d\n", transacciones_size);
    return 0;
}