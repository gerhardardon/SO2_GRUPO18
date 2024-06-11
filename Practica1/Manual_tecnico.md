
# ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ Manual tecnico
|Nombre  | Carnet | 
|------------- | -------------|
| Cristian Daniel Gomez Escobar |202107190 |
| Gerhard Benjamin Ardon Valdez  |202004796 |

##  Descripcion de la practica
En los sistemas de computación, se conoce como un proceso hijo a un proceso creado por otro
proceso (el proceso padre), con la finalidad de realizar subtareas. Este proceso padre debe de
ser capaz de comunicarse y poder monitorear las acciones que realiza el proceso hijo para
asegurarse este ejecutándose correctamente.
La practica consiste en escribir un programa que sea capaz de monitorear y almacenar en un log
todas las llamadas de sistema realizadas por los procesos hijos que ha creado.

## HERRAMIENTAS UTILIZADAS 
- Lenguaje C
	- Proces padre e hijos 
	- Tuberias 
	- Llamadas al sistema 
	- Manejo de ficheros
- Systemtap
	- Monitoreo de rad y write segun PID
	- Manejo de ficheros 


## CREACION DE HIJO 1
Aquí el proceso padre crea un proceso hijo (child1). En este proceso hijo, se cierra el extremo de lectura de la tubería y se redirige la salida estándar (stdout) al extremo de escritura de la tubería. Luego, se ejecuta el programa ./child1 usando execlp.
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


## CREACION DE HIJO 2
Similarmente, el proceso padre crea otro proceso hijo (child2). En este proceso hijo, se cierra el extremo de lectura de la tubería y se redirige la salida estándar (stdout) al extremo de escritura de la segunda tubería. Luego, se ejecuta el programa ./child2 usando execlp.


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

## CREACION DE PROCESO PADRE
El proceso padre es el encargado de la creacion de los hijos, ademas de recibir las llamadas realizadas con las pipes, se podria decir que unicamente desencadena la ejecucion de los hijos 

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
   ```
## CREACION DE TUBERIAS
Aquí se crean dos tuberías (pipe_fd y pipe_rh). Estas tuberías se utilizan para la comunicación entre el proceso padre y los procesos hijos.

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
     
## SYSTEMTAP 

SystemTap es una herramienta de análisis y depuración de sistemas que permite a los administradores y desarrolladores de sistemas obtener información detallada sobre la ejecución del kernel y las aplicaciones de usuario. Es útil para diagnosticar problemas de rendimiento, identificar cuellos de botella y analizar el comportamiento del sistema en tiempo real.

### Características Principales

-   **Análisis en tiempo real**: Permite el monitoreo y la recolección de datos en tiempo real sin necesidad de modificar el código fuente.
-   **Flexibilidad**: Proporciona un lenguaje de scripting para crear scripts personalizados que recogen datos específicos.
-   **Bajo impacto**: Diseñado para minimizar el impacto en el rendimiento del sistema.
-   **Extensible**: Compatible con módulos y bibliotecas adicionales para ampliar su funcionalidad.

## Instalación en Linux Mint

Para instalar SystemTap en Linux Mint, sigue estos pasos:
Antes de comenzar la instalación, asegúrate de que tu sistema esté actualizado.

    sudo apt update
     sudo apt upgrade
     sudo apt install systemtap systemtap-sdt-dev
     sudo apt install linux-headers-$(uname -r)
    stap -V

El script de systemtap que se realizó captura todas las llamadas READ y WRITE de dos procesos, obteniendo su PID, ademas, estos son escritos en un archivo llamado Syscall.log 

    #!/usr/bin/stap
    
    
    probe syscall.read {
        if (pid() == $1 || pid() == $2) {
            // Obtener la hora actual en segundos desde el epoch
            seconds = gettimeofday_s()
            
            // Formatear la hora en una cadena legible
            current_time = ctime(seconds)
            
            // Imprimir la hora actual junto con el mensaje
            printf("Proceso<%d>: read (%s)\n", pid(), current_time)
        }
    }
    
    probe syscall.write {
        if (pid() == $1 || pid() == $2) {
            // Obtener la hora actual en segundos desde el epoch
            seconds = gettimeofday_s()
            
            // Formatear la hora en una cadena legible
            current_time = ctime(seconds)
            
            // Imprimir la hora actual junto con el mensaje
            printf("Proceso<%d>: write (%s)\n", pid(), current_time)
        }
    }
