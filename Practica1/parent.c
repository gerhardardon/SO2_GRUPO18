#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

void setup_log_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error al abrir o crear el archivo");
        exit(1);
    }
    fclose(file);
}

int main() {
    const char *log_filename = "syscalls.log";
    setup_log_file(log_filename);

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(1);
    }


    int pipe_rh[2];
    if (pipe(pipe_rh) == -1) {
        perror("pipe");
        exit(1);
    }



    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) {
        close(pipe_fd[1]);  

        // Primer proceso hijo
        printf("primer proceso hijo - PID: %d, PPID: %d\n", getpid(), getppid());

        // Leer los datos de la tubería
        char buffer[128];
        ssize_t num_bytes = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
        if (num_bytes > 0) {
            buffer[num_bytes] = '\0';
            printf("primer proceso hijo recibió: %s\n", buffer);
        }
        close(pipe_fd[0]);  // Close the read end of the pipe

        // Segunda llamada a fork dentro del primer hijo
        pid2 = fork();
        if (pid2 < 0) {
            perror("fork");
            exit(1);
        } else if (pid2 == 0) {
            // Segundo proceso hijo
            close(pipe_rh[1]);

            printf("segundo proceso hijo - PID: %d, PPID: %d\n", getpid(), getppid());


            // Leer los datos de la tubería
            char bufferh[128];
            ssize_t num_bytesh = read(pipe_rh[0], bufferh, sizeof(bufferh) - 1);
            if (num_bytesh > 0) {
                bufferh[num_bytesh] = '\0';
                printf("segundo proceso hijo recibió: %s\n", bufferh);
            }
            close(pipe_rh[0]);  
            // Close the read end of the pipe


        } else {
            // Código del primer hijo después de crear el segundo hijo
            printf("primer proceso hijo después de crear el segundo hijo - PID: %d, PID del segundo hijo: %d\n", getpid(), pid2);
        }
    } else {
        close(pipe_fd[0]);
        close(pipe_rh[0]);

        // Proceso padre
        printf("proceso padre - PID: %d, PID del primer hijo: %d\n", getpid(), pid1);

        pid_t pid_child1 = fork();
    
        if (pid_child1 < 0) {
            perror("fork");
            exit(1);
        } else if (pid_child1 == 0) {
            // Proceso hijo que ejecuta child1
            dup2(pipe_fd[1], STDOUT_FILENO);  // Redirigir stdout a la tubería
            close(pipe_fd[1]);
            execlp("./child1", "./child1", NULL);
            perror("execlp");
            exit(1);
        } else {
            close(pipe_fd[1]);  // Close the write end of the pipe in the parent after fork
            wait(NULL);  // Esperar a que termine child1
        }

        pid_t pid_child2 = fork();

        if (pid_child2 < 0) {
            perror("fork");
            exit(1);
        } else if (pid_child2 == 0) {
            // Proceso hijo que ejecuta child2
            dup2(pipe_rh[1], STDOUT_FILENO);  // Redirigir stdout a la tubería
            close(pipe_rh[1]);
            execlp("./child2", "./child2", NULL);
            perror("execlp");
            exit(1);
        } else {
            close(pipe_rh[1]);  // Close the write end of the pipe in the parent after fork
            wait(NULL);  // Esperar a que termine child1
        }

    }

    return 0;
}