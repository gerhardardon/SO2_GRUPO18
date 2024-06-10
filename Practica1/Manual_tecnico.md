# ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ Manual tecnico
|Nombre  | Carnet | 
|------------- | -------------|
| Cristian Daniel Gomez Escobar |202107190 |
| Gerhard Benjamin Ardon Valdez  |202004796 |
## CREACION DE TUBERIAS
```
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

```
Aquí se crean dos tuberías (pipe_fd y pipe_rh). Estas tuberías se utilizan para la comunicación entre el proceso padre y los procesos hijos.

## CREACION DE HIJO 1
```
    pid_t pid_child1 = fork();
    if (pid_child1 < 0) {
        perror("fork");
        exit(1);
    } else if (pid_child1 == 0) {
        //printf("Child1 PID %d\n", getpid());
        close(pipe_fd[0]);  // Cerrar el extremo de lectura de la tubería.
        dup2(pipe_fd[1], STDOUT_FILENO);  // Redirigir salida estándar a la tubería
        close(pipe_fd[1]);
        execlp("./child1", "./child1", NULL);
        perror("execlp");
        exit(1);
    }
```
Aquí el proceso padre crea un proceso hijo (child1). En este proceso hijo, se cierra el extremo de lectura de la tubería y se redirige la salida estándar (stdout) al extremo de escritura de la tubería. Luego, se ejecuta el programa ./child1 usando execlp.

## CREACION DE HIJO 2

```
    pid_t pid_child2 = fork();
    if (pid_child2 < 0) {
        perror("fork");
        exit(1);
    } else if (pid_child2 == 0) {
        //printf("Child2 PID %d\n", getpid());
        close(pipe_rh[0]);  // Cerrar el extremo de lectura de la tubería.
        dup2(pipe_rh[1], STDOUT_FILENO);  // Redirigir salida estándar a la tubería
        close(pipe_rh[1]);
        execlp("./child2", "./child2", NULL);
        perror("execlp");
        exit(1);
    }
```
Similarmente, el proceso padre crea otro proceso hijo (child2). En este proceso hijo, se cierra el extremo de lectura de la tubería y se redirige la salida estándar (stdout) al extremo de escritura de la segunda tubería. Luego, se ejecuta el programa ./child2 usando execlp.

## CREACION DE PROCESO PADRE

```
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
```
El proceso padre cierra los extremos de escritura de ambas tuberías y entra en un bucle infinito donde utiliza select para esperar datos de cualquiera de las dos tuberías.
