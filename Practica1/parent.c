#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <time.h>  
#include <signal.h>

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

void print_with_timestamp(pid_t pid, const char *message) {
    time_t now = time(NULL);
    char time_str[64];
    struct tm *timeinfo = localtime(&now);

    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("Proceso <%d>: %s (%s)\n", pid, message, time_str);
    if (log_file != NULL) {
        fprintf(log_file, "Proceso <%d>: %s (%s)\n", pid, message, time_str);
        fflush(log_file);  // Garantizar la escritura inmediata en el archivo
    }
}

void handle_sigint(int sig) {
    printf("\nContadores:\n");
    printf("numero de llamadas del hijo 1: %d\n", c_child1);
    printf("numero de llamadas del hijo 2: %d\n", c_child2);
    printf("write: %d\n", c_write);
    printf("open: %d\n", c_open);
    printf("read: %d\n", c_read);
    printf("PROCESOS FINALIZADOS\n");
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
        printf("Child1 PID %d\n", getpid());
        close(pipe_fd[0]);  // Cerrar el extremo de lectura de la tubería.
        dup2(pipe_fd[1], STDOUT_FILENO);  // Redirigir salida estándar a la tubería
        close(pipe_fd[1]);
        execlp("./child1", "./child1", NULL);
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
        printf("Child2 PID %d\n", getpid());
        close(pipe_rh[0]);  // Cerrar el extremo de lectura de la tubería.
        dup2(pipe_rh[1], STDOUT_FILENO);  // Redirigir salida estándar a la tubería
        close(pipe_rh[1]);
        execlp("./child2", "./child2", NULL);
        perror("execlp");
        exit(1);
    }

// ------------------------------------------------------------------
// ----------------- CREACION DEL PROCESO PADRE -----------------------
    close(pipe_fd[1]);  // Cerrar el extremo de escritura de la tubería.
    close(pipe_rh[1]);  // Cerrar el extremo de escritura del otro tubo.

    char buffer[128];
    char bufferh[128];
    ssize_t num_bytes, num_bytesh;

    fd_set readfds;
    int max_fd = (pipe_fd[0] > pipe_rh[0]) ? pipe_fd[0] : pipe_rh[0];

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(pipe_fd[0], &readfds);
        FD_SET(pipe_rh[0], &readfds);

        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(pipe_fd[0], &readfds)) {
            num_bytes = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
            if (num_bytes > 0) {
                buffer[num_bytes] = '\0';
                print_with_timestamp(pid_child1, buffer);
                c_child1++;
                if (strcmp(buffer, "write") == 0) {
                    c_write++;
                } else if (strcmp(buffer, "open") == 0) {
                    c_open++;
                } else if (strcmp(buffer, "read") == 0) {
                    c_read++;
                }
            }
        }

        if (FD_ISSET(pipe_rh[0], &readfds)) {
            num_bytesh = read(pipe_rh[0], bufferh, sizeof(bufferh) - 1);
            if (num_bytesh > 0) {
                bufferh[num_bytesh] = '\0';
                print_with_timestamp(pid_child2, bufferh);
                c_child2++;
                if (strcmp(bufferh, "write") == 0) {
                    c_write++;
                } else if (strcmp(bufferh, "open") == 0) {
                    c_open++;
                } else if (strcmp(bufferh, "read") == 0) {
                    c_read++;
                }
            }
        }
    }

    // Cerrar los extremos de lectura de las tuberías.
    close(pipe_fd[0]);
    close(pipe_rh[0]);

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
