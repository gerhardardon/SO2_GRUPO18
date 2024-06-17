#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cJSON.h"
#include <string.h>
#include <time.h>
#include "read_json.h"

#define NUM_THREADS 3 //  definimos el numero de hilos
int error_count = 0;

struct erro_struct
{ //  creamos una estructura para guardar los datos de los usuarios
    int linea;
    char descripcion[50];
};
struct data_struct usuarios[200]; //  creamos un arreglo de estructuras para guardar los datos de los usuarios
struct erro_struct errores[200];  //  creamos un arreglo de estructuras para guardar los datos de los usuarios
int usuarios_size = 0;            //  creamos una variable para guardar el tamaño de los usuarios
pthread_mutex_t mutex;

typedef struct
{ //  creamos una estructura para guardar los datos de los hilos
    cJSON *json;
    int start;
    int end;
    int tread_id;
    int count;
} thread_data;

void *process_json_segment(void *arg)
{                                           //  creamos una funcion para procesar el json
    thread_data *data = (thread_data *)arg; //  obtenemos los datos de los hilos
    cJSON *json = data->json;               //  obtenemos el json
    int start = data->start;                //  obtenemos el inicio
    int end = data->end;                    //  obtenemos el final

    int n = start;      //  inicializamos un contador
    cJSON *item = NULL; //  creamos un objeto json
    for (int i = start; i < end; i++)
    {                                       //  iteramos sobre el json
        item = cJSON_GetArrayItem(json, i); //  obtenemos el objeto del json
        if (cJSON_IsObject(item))
        {                                                              //  verificamos que sea un objeto
            cJSON *no_cuenta = cJSON_GetObjectItem(item, "no_cuenta"); //  obtenemos el valor del objeto
            cJSON *nombre_ = cJSON_GetObjectItem(item, "nombre");
            cJSON *saldo_ = cJSON_GetObjectItem(item, "saldo");

            if (cJSON_IsNumber(no_cuenta) && cJSON_IsString(nombre_) && cJSON_IsNumber(saldo_))
            { //  verificamos que los valores sean numericos y string
                // verificar si saldo_ es positivo
                if (saldo_->valuedouble >= 0)
                {

                    pthread_mutex_lock(&mutex); //  bloqueamos el mutex
                    int duplicate = 0;          //  inicializamos una variable para verificar duplicados
                    for (int j = 0; j < usuarios_size; j++)
                    { //  iteramos sobre los usuarios
                        if (usuarios[j].no_cuenta == no_cuenta->valueint)
                        {                  //  verificamos si el usuario ya existe
                            duplicate = 1; //  si existe, marcamos la variable como verdadera
                            break;
                        }
                    }
                    if (!duplicate)
                    { //  si no existe el usuario

                        usuarios[usuarios_size].no_cuenta = no_cuenta->valueint;             //  guardamos los valores en la estructura
                        sprintf(usuarios[usuarios_size].nombre, "%s", nombre_->valuestring); //  guardamos los valores en la estructura
                        usuarios[usuarios_size].saldo = saldo_->valuedouble;                 //  guardamos los valores en la estructura
                        usuarios_size++;                                                     //  incrementamos el tamaño de los usuarios
                        data->count++;                                                       //  incrementamos el contador
                    }
                    else
                    {

                        errores[error_count].linea = i + 1;                               //  guardamos los valores en la estructura
                        sprintf(errores[error_count].descripcion, "no_cuenta duplicado"); //  guardamos los valores en la estructura
                        error_count++;                                                    //  incrementamos el contador de errores
                    }
                    pthread_mutex_unlock(&mutex); //  bloqueamos el mutex
                }
                else
                {

                    errores[error_count].linea = i + 1;                          //  guardamos los valores en la estructura
                    sprintf(errores[error_count].descripcion, "saldo negativo"); //  guardamos los valores en la estructura
                    error_count++;                                               //  incrementamos el contador de errores
                }
            }else {

                errores[error_count].linea = i + 1; //  guardamos los valores en la estructura
                sprintf(errores[error_count].descripcion, "tipo de dato incorrecto"); //  guardamos los valores en la estructura
                error_count++; //  incrementamos el contador de errores
            
                
            }
        }
    }
    pthread_exit(NULL); //  salimos del hilo
}
void log_results(thread_data data[], int total_items)   // guardamos los resultados en un archivo de log
{
    time_t t = time(NULL);  //  obtenemos la fecha y hora actual
    struct tm tm = *localtime(&t);  //  obtenemos la fecha y hora actual

    char filename[100]; //  creamos un arreglo para guardar el nombre del archivo
    sprintf(filename, "carga_%d_%02d_%02d-%02d_%02d_%02d.log",  //  guardamos el nombre del archivo
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    FILE *log_file = fopen(filename, "w");  //  abrimos el archivo de log
    if (!log_file)  //  verificamos que el archivo se haya abierto
    {
        perror("Error al abrir el archivo de log"); //  imprimimos un mensaje de error
        return;
    }

    fprintf(log_file, "----------Carga de usuarios----------\n\n");
    fprintf(log_file, "Fecha: %d-%02d-%02d %02d:%02d:%02d\n\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    fprintf(log_file, "Usuarios Cargados:\n");  //  imprimimos los usuarios cargados
    for (int i = 0; i < NUM_THREADS; i++)   //  iteramos sobre los hilos
    {
        fprintf(log_file, "Hilo #%d: %d\n", i + 1, data[i].count);      //  imprimimos el id del hilo y el contador
    }
    int totalloaded = 0;
    for (int i = 0; i < NUM_THREADS; i++)   //  iteramos sobre los hilos
    {
        totalloaded += data[i].count;   //  sumamos el contador de cada hilo
    }
    fprintf(log_file, "Total: %d\n\n", totalloaded);    //  imprimimos el total de usuarios cargados

    fprintf(log_file, "Errores:\n");    //  imprimimos los errores

    /*  for (int i = 0; i < usuarios_size; i++) {
          for (int j = i + 1; j < usuarios_size; j++) {
              if (usuarios[i].no_cuenta == usuarios[j].no_cuenta) {
                  fprintf(log_file, "\t-Linea %d: 'no_cuenta' duplicado (%d)\n", j + 1, usuarios[j].no_cuenta);
                  error_count++;
              }
          }
      }*/

    for (int i = 0; i < error_count; i++)   
    {
        fprintf(log_file, "\t-Linea %d: %s\n", errores[i].linea, errores[i].descripcion);
    }
    if (error_count == 0)
    {
        fprintf(log_file, "\tNo se encontraron errores\n");
    }

    fclose(log_file);
}





void read_json_file(char *filename)
{ //  creamos una funcion para leer el archivo json
    // vaciamor el contador de errores
    error_count = 0;

    FILE *file = fopen(filename, "r"); //  abrimos el archivo
    if (!file)
    { //  verificamos que el archivo exista
        perror("Open File");
        return;
    }

    fseek(file, 0, SEEK_END); //  obtenemos el tamaño del archivo
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(filesize + 1); //  creamos un buffer para leer el archivo
    fread(buffer, 1, filesize, file);            //  leemos el archivo
    buffer[filesize] = '\0';                     //  agregamos un caracter nulo al final del buffer

    fclose(file); //  cerramos el archivo

    cJSON *json = cJSON_Parse(buffer); //  parseamos el json
    if (json == NULL)
    { //  verificamos que el json no sea nulo
        perror("Parse JSON");
        return;
    }

    if (!cJSON_IsArray(json))
    { //  verificamos que el json sea un array
        perror("JSON no es un array");
        return;
    }

    int total_items = cJSON_GetArraySize(json);       //  obtenemos el tamaño del json
    int items_per_thread = total_items / NUM_THREADS; //  obtenemos el tamaño de los items por hilo, para dividir el trabajo
    pthread_t threads[NUM_THREADS];                   //  creamos un arreglo de hilos
    thread_data data[NUM_THREADS];                    //  creamos un arreglo de estructuras para guardar los datos de los hilos

    pthread_mutex_init(&mutex, NULL); //  inicializamos el mutex

    for (int i = 0; i < NUM_THREADS; i++)
    {                                                                                    //  iteramos sobre los hilos
        data[i].json = json;                                                             //  guardamos el json en la estructura
        data[i].start = i * items_per_thread;                                            //  guardamos el inicio en la estructura
        data[i].end = (i == NUM_THREADS - 1) ? total_items : (i + 1) * items_per_thread; //  guardamos el final en la estructura, si es el ultimo hilo guardamos el total de items
        data[i].tread_id = i + 1;                                                        //  guardamos el id del hilo
        data[i].count = 0;                                                               //  inicializamos el contador
        pthread_create(&threads[i], NULL, process_json_segment, (void *)&data[i]);       //  creamos el hilo
    }

    for (int i = 0; i < NUM_THREADS; i++)
    { //  iteramos sobre los hilos
        pthread_join(threads[i], NULL);
    }
    log_results(data, total_items); //  guardamos los resultados en un archivo de log

    // usuarios_size = total_items; //  guardamos el tamaño de los usuarios

    pthread_mutex_destroy(&mutex); //  destruimos el mutex
    cJSON_Delete(json);            //  eliminamos el json
    free(buffer);                  //  liberamos la memoria

    // mostramos los usuarios del array
    for (int i = 0; i < usuarios_size; i++)
    { //  imprimimos los datos de los usuarios
        printf("no_cuenta: %d   nombre: %s  saldo: %2f \n",
               usuarios[i].no_cuenta, usuarios[i].nombre, usuarios[i].saldo);
    }
}

//creamos una funcion que convierte la estructura a json en reporte.json los datos de la lista usuarios

char* struct_to_json(){

    // Create an empty JSON Array
    cJSON *json_array = cJSON_CreateArray();

    // Iterate through the structs
    for(int i = 0; i < usuarios_size; i++){
        // Create an emty JSON Object
        cJSON *item = cJSON_CreateObject();

        // Add the values to the object
        cJSON_AddNumberToObject(item, "no_cuenta", usuarios[i].no_cuenta);
        cJSON_AddStringToObject(item, "nombre", usuarios[i].nombre);
        cJSON_AddNumberToObject(item, "saldo", usuarios[i].saldo);

        // Add the opbject to the Array
        cJSON_AddItemToArray(json_array, item);
    }

    // Format the JSON Array 
    char* formatted = cJSON_Print(json_array);
    return formatted;
}

void write_file(char* filename, char* data){
    //Open the file
    FILE *file = fopen(filename, "wb");
    if(file == NULL){
        perror("Open file");
        return;
    }

    // Write the data
    fwrite(data, 1, strlen(data), file);
    fclose(file);
}
void reporte_estado_cuenta(){
    char* new_json = struct_to_json();  //  convertimos la estructura a json
    write_file("Estado_de_cuentas.json", new_json);   //  escribimos el json en un archivo
}
