#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
int main() {
    srand(time(NULL)^ getpid());
    /*
    const char *params = "Open";
    int pipe_rh = 1;  
    write(pipe_rh, params, strlen(params));
    close(pipe_rh);
    return 0;
    */
    // loop infinito
  /* */ 
  
   while (1)
    {
        // random number de 1 a 3 
        int random = rand() % 3 + 1;
        if (random == 1){
            printf("-open\n");
            //enviar mensaje a padre

        } else if (random == 2){
            printf("-write\n");
            //abrir archivo
            FILE *file = fopen("practica1.txt", "a");
            if (file == NULL) {
                perror("Error al abrir o crear el archivo");
                exit(1);
            }
            // generar aleatorio de 8 caracteres
            char contenido[] = "abcdefghijklmnopqrstuvwxyz0123456789";
            char cadena[8];
            for (int i = 0; i < 8; i++) {
                cadena[i] = contenido[rand() % (sizeof(contenido) - 1)];
            }
            cadena[8] = '\0';
            // escribir en archivo
            fprintf(file, "%s\n", cadena);
            fclose(file);
            //enviar mensaje a padre

        } else {
            printf("-read\n");
            //abrir archivo en modo lectura
            FILE *file = fopen("practica1.txt", "r");
            if (file == NULL) {
                perror("Error al abrir o crear el archivo");
                exit(1);
            }
            // leer ultimos 8 caracteres
            char cadena[8];
            fseek(file, -9, SEEK_END);
            fscanf(file, "%s", cadena);
            printf("-%s\n", cadena);
            fclose(file);
            //enviar mensaje a padre
        }

        sleep(random);
    }
    return 0;
    
}
