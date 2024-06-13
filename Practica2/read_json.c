#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cJSON.h"
#include <string.h>
#include "read_json.h"

#define NUM_THREADS 3 //  definimos el numero de hilos

struct data_struct { //  creamos una estructura para guardar los datos de los usuarios
    int no_cuenta; 
    char nombre[50];
    float saldo;
};
struct data_struct usuarios[200]; //  creamos un arreglo de estructuras para guardar los datos de los usuarios
int usuarios_size = 0; //  creamos una variable para guardar el tamaño de los usuarios
pthread_mutex_t mutex;

typedef struct { //  creamos una estructura para guardar los datos de los hilos
    cJSON *json;
    int start;
    int end;
} thread_data;

void *process_json_segment(void *arg) { //  creamos una funcion para procesar el json
    thread_data *data = (thread_data *)arg; //  obtenemos los datos de los hilos
    cJSON *json = data->json; //  obtenemos el json
    int start = data->start;    //  obtenemos el inicio
    int end = data->end;    //  obtenemos el final

    int n = start;  //  inicializamos un contador
    cJSON *item = NULL; //  creamos un objeto json
    for (int i = start; i < end; i++) { //  iteramos sobre el json
        item = cJSON_GetArrayItem(json, i); //  obtenemos el objeto del json
        if (cJSON_IsObject(item)) { //  verificamos que sea un objeto
            cJSON *no_cuenta = cJSON_GetObjectItem(item, "no_cuenta");  //  obtenemos el valor del objeto
            cJSON *nombre_ = cJSON_GetObjectItem(item, "nombre");   
            cJSON *saldo_ = cJSON_GetObjectItem(item, "saldo");  

            if (cJSON_IsNumber(no_cuenta) && cJSON_IsString(nombre_) && cJSON_IsNumber(saldo_)) { //  verificamos que los valores sean numericos y string
                pthread_mutex_lock(&mutex); //  bloqueamos el mutex, para evitar problemas de concurrencia
                usuarios[n].no_cuenta = no_cuenta->valueint;    //  guardamos los valores en la estructura
                sprintf(usuarios[n].nombre, "%s", nombre_->valuestring);    //  guardamos los valores en la estructura
                usuarios[n].saldo = saldo_->valuedouble;   //  guardamos los valores en la estructura
                n++;
                pthread_mutex_unlock(&mutex);   //  desbloqueamos el mutex
            }
        }
    }
    pthread_exit(NULL); //  salimos del hilo
}

void read_json_file(char *filename) {   //  creamos una funcion para leer el archivo json
    FILE *file = fopen(filename, "r");  //  abrimos el archivo
    if (!file) {    //  verificamos que el archivo exista
        perror("Open File");    
        return;
    }

    fseek(file, 0, SEEK_END);   //  obtenemos el tamaño del archivo
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);   

    char *buffer = (char *)malloc(filesize + 1);    //  creamos un buffer para leer el archivo
    fread(buffer, 1, filesize, file);   //  leemos el archivo
    buffer[filesize] = '\0';   //  agregamos un caracter nulo al final del buffer

    fclose(file);   //  cerramos el archivo

    cJSON *json = cJSON_Parse(buffer);  //  parseamos el json
    if (json == NULL) { //  verificamos que el json no sea nulo
        perror("Parse JSON");
        return;
    }

    if (!cJSON_IsArray(json)) { //  verificamos que el json sea un array    
        perror("JSON no es un array");
        return;
    }

    int total_items = cJSON_GetArraySize(json); //  obtenemos el tamaño del json
    int items_per_thread = total_items / NUM_THREADS;   //  obtenemos el tamaño de los items por hilo, para dividir el trabajo
    pthread_t threads[NUM_THREADS]; //  creamos un arreglo de hilos
    thread_data data[NUM_THREADS];  //  creamos un arreglo de estructuras para guardar los datos de los hilos

    pthread_mutex_init(&mutex, NULL);   //  inicializamos el mutex

    for (int i = 0; i < NUM_THREADS; i++) { //  iteramos sobre los hilos
        data[i].json = json;    //  guardamos el json en la estructura
        data[i].start = i * items_per_thread;   //  guardamos el inicio en la estructura
        data[i].end = (i == NUM_THREADS - 1) ? total_items : (i + 1) * items_per_thread;    //  guardamos el final en la estructura
        pthread_create(&threads[i], NULL, process_json_segment, (void *)&data[i]);  //  creamos el hilo
    }

    for (int i = 0; i < NUM_THREADS; i++) { //  iteramos sobre los hilos
        pthread_join(threads[i], NULL);
    }

    usuarios_size = total_items; //  guardamos el tamaño de los usuarios

    pthread_mutex_destroy(&mutex); //  destruimos el mutex
    cJSON_Delete(json); //  eliminamos el json
    free(buffer);   //  liberamos la memoria

        for(int i = 0; i < usuarios_size; i++){ //  imprimimos los datos de los usuarios
        printf("no_cuenta: %d   nombre: %s  saldo: %2f \n",   
            usuarios[i].no_cuenta, usuarios[i].nombre, usuarios[i].saldo);
    }
