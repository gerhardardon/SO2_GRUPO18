#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <time.h>  
#include <signal.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

FILE *log_file;

int c_child1 = 0;
int c_child2 = 0;
int c_write = 0;
int c_open = 0;
int c_read = 0;

void setup_log_file(const char *filename) {
    log_file = fopen(filename, "w");
    if (log_file == NULL) {
        perror("Error al abrir o crear el archivo");
        exit(1);
    }
}

void close_log_file() {
    if (log_file != NULL) {
        fclose(log_file);
    }
}

int count_word_in_line(const char *line, const char *word) {
    int count = 0;
    const char *tmp = line;
    int word_len = strlen(word);

    while ((tmp = strstr(tmp, word)) != NULL) {
        if ((tmp == line || !isalnum(tmp[-1])) && !isalnum(tmp[word_len])) {
            count++;
        }
        tmp += word_len;
    }
    return count;
}

void print_with_timestamp(pid_t pid, const char *message) {
    time_t now = time(NULL);
    char time_str[64];
    struct tm *timeinfo = localtime(&now);

    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    if(pid == 94627){
        printf("\033[1;91mProceso <%d>\033[0m: %s (%s)\n", pid, message, time_str);
    }else if(pid == 94628){
        printf("\033[1;36mProceso <%d>\033[0m: %s (%s)\n", pid, message, time_str);
    }
   // printf("Proceso <%d>: %s (%s)\n", pid, message, time_str);
    if (log_file != NULL) {
        fprintf(log_file, "Proceso <%d>: %s (%s)\n", pid, message, time_str);
        fflush(log_file);  // Garantizar la escritura inmediata en el archivo
    }
}

void handle_sigint(int sig) {
    const char *filename = "syscalls.log"; // nombre
    const char *word2 = "read"; // palabra a buscar
    const char *word = "write"; // palabra a buscar

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    int word_count = 0;
    int word2_count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_count++;
        word_count += count_word_in_line(line, word);
        word2_count += count_word_in_line(line, word2);
    }

    fclose(file);
    
    
    
    printf("\n\n\033[1;32m\t\tContadores systap\033[0m\n");
    printf("\033[1mnumero de llamadas al sistema\033[0m: %d\n", line_count);  
    printf("\033[1m%s\033[0m: %d \n", word, word_count);
    printf("\033[1m%s\033[0m: %d \n", word2, word2_count);
    close_log_file();
    exit(0);
}

int main() {
    const char *log_filename = "syscalls.log";
    setup_log_file(log_filename);

    // Configurar el controlador SIGINT
    signal(SIGINT, handle_sigint);

  // ----------------- CREACION E TUBERIAS-----------------------
    int pipe_fd[2];
    int pipe_rh[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(1);
    }

    if (pipe(pipe_rh) == -1) {
        perror("pipe");
        exit(1);
    }

// ----------------------------------------------------------
// ----------------- CREACION DEL PROCESO HIJO 1 -----------------------
    pid_t pid_child1 = fork();
    if (pid_child1 < 0) {
        perror("fork");
        exit(1);
    } else if (pid_child1 == 0) {
        //printf("Child1 PID %d\n", getpid());
        close(pipe_fd[0]);  // Cerrar el extremo de lectura de la tubería.
        dup2(pipe_fd[1], STDOUT_FILENO);  // Redirigir salida estándar a la tubería
        close(pipe_fd[1]);
        execlp("./child1.bin", "./child1.bin", NULL);
        perror("execlp");
        exit(1);
    }

// -------------------------------------------------------------------
// ----------------- CREACION DEL PROCESO HIJO 2-----------------------
    pid_t pid_child2 = fork();
    if (pid_child2 < 0) {
        perror("fork");
        exit(1);
    } else if (pid_child2 == 0) {
        //printf("Child2 PID %d\n", getpid());
        close(pipe_rh[0]);  // Cerrar el extremo de lectura de la tubería.
        dup2(pipe_rh[1], STDOUT_FILENO);  // Redirigir salida estándar a la tubería
        close(pipe_rh[1]);
        execlp("./child2.bin", "./child2.bin", NULL);
        perror("execlp");
        exit(1);
    }

// ------------------------------------------------------------------
// ----------------- CREACION DEL PROCESO PADRE -----------------------
    close(pipe_fd[1]);  // Cerrar el extremo de escritura de la tubería.
    close(pipe_rh[1]);  // Cerrar el extremo de escritura del otro tubo.

    printf("\033[1;95mParent PID %d\033[0m\n", getpid());
    printf("\t\033[1;91mhijo1 PID %d\033[0m\n", pid_child1);
    printf("\t\033[1;36mhijo2 PID %d\033[0m\n", pid_child2);
    // Define el archivo de salida
    const char* output_file = "syscalls.log";

    // Construye el comando para ejecutar el script de SystemTap con los PIDs como argumentos y redirigir la salida a un archivo
    char command[256];
    snprintf(command, sizeof(command), "sudo stap systap.stp %d %d > %s", pid_child1, pid_child2, output_file);

    // Ejecuta el comando
    int result = system(command);

 

    // Espere a que finalicen los procesos secundarios
    wait(NULL);
    wait(NULL);
   
    close_log_file();

    return 0;
// ------------------------------------------------------------------
}

/*
gcc parent.c -o parent
./parent
*/




/*
gcc parent.c -o parent
./parent
*/
