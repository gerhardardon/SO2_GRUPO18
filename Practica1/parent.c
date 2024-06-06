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

    pid_t pid1, pid2;

    // primera llamada a fork
    pid1 = fork();

    if (pid1 < 0) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) {
        // primer proceso hijo
        printf("primer proceso hijo - PID: %d, PPID: %d\n", getpid(), getppid());

        // segunda llamada a fork dentro del primer hijo
        pid2 = fork();

        if (pid2 < 0) {
            perror("fork");
            exit(1);
        } else if (pid2 == 0) {
            //segundo proceso hijo
            printf("segundo proceso hijo - PID: %d, PPID: %d\n", getpid(), getppid());
        } else {
            // código del primer hijo después de crear el segundo hijo
            printf("primer proceso hijo después de crear el segundo hijo - PID: %d, PID del segundo hijo: %d\n", getpid(), pid2);
        }
    } else {
        // proceso padre
        printf("proceso padre - PID: %d, PID del primer hijo: %d\n", getpid(), pid1);
        
    }

    return 0;
}

/*
                    COMANDOS DE EJECUCION

 gcc parent.c -o parent
 ./parent

*/
