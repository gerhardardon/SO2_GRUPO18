# Manual tecnico
|Nombre  | Carnet | 
|------------- | -------------|
| Cristian Daniel Gomez Escobar |202107190 |
| Gerhard Benjamin Ardon Valdez  |202004796 |

##  Descripcion de la practica2 
Las corporaciones utilizan computadoras de alto rendimiento llamados mainframes para
aplicaciones que dependen de la escalabilidad y la confiabilidad. Por ejemplo, una institución
bancaria podría utilizar un mainframe para albergar la base de datos de las cuentas de sus
clientes, para las cuales se pueden enviar transacciones desde cualquiera de los miles de cajeros
automáticos en todo el mundo.
La práctica consiste en realizar una aplicación en consola en C que permita almacenar los datos
de usuario de un banco, así como poder realizar operaciones monetarias como depósitos,
retiros y transacciones.

En los sistemas de computación, se conoce como un proceso hijo a un proceso creado por otro
proceso (el proceso padre), con la finalidad de realizar subtareas. Este proceso padre debe de
ser capaz de comunicarse y poder monitorear las acciones que realiza el proceso hijo para
asegurarse este ejecutándose correctamente.
La practica consiste en escribir un programa que sea capaz de monitorear y almacenar en un log
todas las llamadas de sistema realizadas por los procesos hijos que ha creado.

## HERRAMIENTAS UTILIZADAS 
- Lenguaje C
	- hilos
	- pthreads 
	- mutex
	- semaforos
	- cJSON

## Main

en este archivo se contiene el menu principal de la aplicacion en consola con las operaciones de 
- carga masiva de usuarios
- carga masiva de operaciones 
- operaciones individuales 
- reportes de estados de cuenta 
- creacion automatica de logs de cargas masivas para revisar la informacio
- opcion de salir 
  

### Pthreads

#### Descripción
La biblioteca Pthreads (POSIX threads) proporciona una interfaz estándar para la programación concurrente en sistemas que cumplen con POSIX. Permite la creación y gestión de hilos (threads), lo que facilita la ejecución de múltiples tareas simultáneamente dentro de un programa.

#### Funciones Comunes
- **pthread_create**: Crea un nuevo hilo.
  ```c
  int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
  ```
- **pthread_join**: Espera a que un hilo termine.
  ```c
  int pthread_join(pthread_t thread, void **retval);
  ```
- **pthread_exit**: Termina la ejecución del hilo.
  ```c
  void pthread_exit(void *retval);
  ```
- **pthread_mutex_init**: Inicializa un mutex.
  ```c
  int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
  ```
- **pthread_mutex_lock**: Bloquea un mutex.
  ```c
  int pthread_mutex_lock(pthread_mutex_t *mutex);
  ```
- **pthread_mutex_unlock**: Desbloquea un mutex.
  ```c
  int pthread_mutex_unlock(pthread_mutex_t *mutex);
  ```
- **pthread_mutex_destroy**: Destruye un mutex.
  ```c
  int pthread_mutex_destroy(pthread_mutex_t *mutex);
  ```

### Mutex

#### Descripción
Un mutex (mutual exclusion) es un mecanismo de sincronización utilizado para prevenir el acceso concurrente a un recurso compartido. Solo un hilo puede poseer el mutex a la vez, asegurando que las secciones críticas de código se ejecuten de manera segura.

#### Uso Común
- **Inicialización**: 
  ```c
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  ```
- **Bloqueo y desbloqueo**:
  ```c
  pthread_mutex_lock(&mutex);
  // Sección crítica
  pthread_mutex_unlock(&mutex);
  ```
- **Destrucción**:
  ```c
  pthread_mutex_destroy(&mutex);
  ```

### Semáforos

#### Descripción
Los semáforos son otra herramienta de sincronización utilizada para controlar el acceso a uno o más recursos. Un semáforo puede ser binario (0 o 1) o contar (0 a N), permitiendo controlar un número específico de recursos.

#### Funciones Comunes (semaphores.h)
- **sem_init**: Inicializa un semáforo.
  ```c
  int sem_init(sem_t *sem, int pshared, unsigned int value);
  ```
- **sem_wait**: Disminuye el valor del semáforo, bloqueando si es 0.
  ```c
  int sem_wait(sem_t *sem);
  ```
- **sem_post**: Incrementa el valor del semáforo.
  ```c
  int sem_post(sem_t *sem);
  ```
- **sem_destroy**: Destruye un semáforo.
  ```c
  int sem_destroy(sem_t *sem);
  ```

### cJSON

#### Descripción
cJSON es una biblioteca en C para trabajar con JSON (JavaScript Object Notation). Facilita la creación, modificación y análisis de datos en formato JSON.

#### Funciones Comunes
- **cJSON_Parse**: Parsea una cadena JSON y la convierte en una estructura cJSON.
  ```c
  cJSON *cJSON_Parse(const char *value);
  ```
- **cJSON_Print**: Convierte una estructura cJSON en una cadena JSON.
  ```c
  char *cJSON_Print(const cJSON *item);
  ```
- **cJSON_GetObjectItem**: Obtiene un elemento de un objeto JSON por su nombre.
  ```c
  cJSON *cJSON_GetObjectItem(const cJSON *object, const char *string);
  ```
- **cJSON_Delete**: Libera la memoria ocupada por una estructura cJSON.
  ```c
  void cJSON_Delete(cJSON *c);
  ```

#### Ejemplo de Uso
```c
#include "cJSON.h"

// Crear un objeto JSON
cJSON *root = cJSON_CreateObject();
cJSON_AddStringToObject(root, "name", "John Doe");
cJSON_AddNumberToObject(root, "age", 30);

// Convertir el objeto JSON a una cadena
char *json_string = cJSON_Print(root);
printf("JSON Output: %s\n", json_string);

// Liberar la memoria
cJSON_Delete(root);
free(json_string);
```

### Recursos Adicionales

- **Documentación de Pthreads**: [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- **Documentación de cJSON**: [cJSON GitHub Repository](https://github.com/DaveGamble/cJSON)

Este resumen proporciona una visión general básica sobre el uso de Pthreads, mutex, semáforos y cJSON, junto con ejemplos prácticos para cada uno.