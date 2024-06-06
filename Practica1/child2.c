#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Estos son los parámetros que queremos enviar
    const char *params = "Open";

    // Escribimos los parámetros a la tubería
    int pipe_fd = 1;  // Asumimos que el descriptor de archivo 1 es la escritura de la tubería

    write(pipe_fd, params, strlen(params));
    close(pipe_fd);

    return 0;
}
