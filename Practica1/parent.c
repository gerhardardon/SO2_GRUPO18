#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

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
    int pipe_rh[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(1);
    }

    if (pipe(pipe_rh) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid_child1 = fork();
    if (pid_child1 < 0) {
        perror("fork");
        exit(1);
    } else if (pid_child1 == 0) {
        // Proceso hijo que ejecuta child1
        close(pipe_fd[0]);  // Cerrar el extremo de lectura de la tubería
        dup2(pipe_fd[1], STDOUT_FILENO);  // Redirigir stdout a la tubería
        close(pipe_fd[1]);
        execlp("./child1", "./child1", NULL);
        perror("execlp");
        exit(1);
    }

    pid_t pid_child2 = fork();
    if (pid_child2 < 0) {
        perror("fork");
        exit(1);
    } else if (pid_child2 == 0) {
        // Proceso hijo que ejecuta child2
        close(pipe_rh[0]);  // Cerrar el extremo de lectura de la tubería
        dup2(pipe_rh[1], STDOUT_FILENO);  // Redirigir stdout a la tubería
        close(pipe_rh[1]);
        execlp("./child2", "./child2", NULL);
        perror("execlp");
        exit(1);
    }

    // Proceso padre
    close(pipe_fd[1]);  // Cerrar el extremo de escritura de la tubería
    close(pipe_rh[1]);  // Cerrar el extremo de escritura de la tubería

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
                printf("proceso padre recibió de child1: %s\n", buffer);
            }
        }

        if (FD_ISSET(pipe_rh[0], &readfds)) {
            num_bytesh = read(pipe_rh[0], bufferh, sizeof(bufferh) - 1);
            if (num_bytesh > 0) {
                bufferh[num_bytesh] = '\0';
                printf("proceso padre recibió de child2: %s\n", bufferh);
            }
        }
    }

    // Esperar a que terminen los procesos hijos (si fuese necesario)
    close(pipe_fd[0]);
    close(pipe_rh[0]);
    wait(NULL);
    wait(NULL);

    return 0;
}
