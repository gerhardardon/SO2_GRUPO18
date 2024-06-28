
# Manual tecnico
|Nombre  | Carnet | 
|------------- | -------------|
| Cristian Daniel Gomez Escobar |202107190 |
| Gerhard Benjamin Ardon Valdez  |202004796 |
# Descripcion del Proyecto
La gestión de la memoria es un aspecto crítico del rendimiento del sistema, ya que garantiza la utilización eficiente de los recursos y mantiene la estabilidad bajo diferentes cargas de trabajo. Al monitorear exhaustivamente la memoria, se puede obtener información valiosa sobre el comportamiento de su
sistema, identificar posibles cuellos de botella y optimizar la asignación de recursos. En este proyecto se creara una aplicación capaz de monitorear el uso de memoria de cada proceso abierto en Linux, detectando las solicitudes de memoria que estos realizan al sistema operativo.

## Herramientas utilizadas 
- systemtap
- C 
- Python
- Flask
- Node
- React+Vite

## Systemtap 
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
    
Se utilizo el script de systemtap para capturar todas las llamadas mmap y munmap que son realizadas por el kernel. 
este script cuetnta con el nombre systap.stp y para compilarlo se necesita el comando 
```
sudo stap systap.stp
```

Codigo para capturar mmap 
```

probe syscall.mmap2 {
    if (execname() != "cat"){
        printf("%d %s mmap %d %s\n",
        pid(), execname(), length, ctime())
    }
}
```
Codigo para capturar munmap
```
probe syscall.munmap {
    if (execname() != "cat"){
        printf("%d %s munmap %d %s\n",
        pid(), execname(), length, ctime())
    }
}
```
## Lector en C
Este programa en C es el encargado de leer la salida del systap 
```
fp = popen("sudo ./systap.stp", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run SystemTap script\n");
        exit(EXIT_FAILURE);
    }
```

y guardar en la base de datos mysql toda la informacion relacionada con los proceso, ademas se utiliza la libreria 
#include <mysql/mysql.h> para poder realizar la conexion con la db.
```
//conectarse a db
MYSQL* connect_to_database() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, "localhost", "root", "root", 
                           "proyecto2", 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    return conn;
}
```
## Api
Para realizar la api que envia datos desde la base de datos mysql se utilizo python y flask para manejar de manera sencilla las conexiones

Esta cuenta con las siguientes funciones de manejo de base de datos:
### get_solicitudes()
La cual obtiene todos los datos de la tabla solicitudes de la db, en esta tabla se guardan los datos de las llamadas mmap y munmap de los procesos
```
# Ejecutar la consulta
    cursor.execute("SELECT * FROM solicitudes")

```
### get_procesos()
La cual obtiene los 10 procesos que consumen mayor cantidad de memoria virtual, para ser desplegada en el dashboard y tambien en la grafica de pie 
```
# Ejecutar la consulta
    cursor.execute("SELECT * FROM procesos ORDER BY pid DESC LIMIT 10;")
```
### get_todos_procesos()
Parecida a la funcion anterior, esta sevuelve todos los procesos para que sean accedidos por la pagina web
```
 # Ejecutar la consulta
    cursor.execute("SELECT * FROM procesos;")
```

Ademas la aplicacion de flask corre en el puerto 5200 y cuernta con los siguentes endpoints para su conexion 
```
@app.route('/api/data/llamadas', methods=['GET'])
def get_llamadas_solicitudes():
    solicitudes = get_solicitudes()
    return jsonify(solicitudes)

@app.route('/api/data/procesos', methods=['GET'])
def get_llamadas_procesos():
    procesos = get_procesos()
    return jsonify(procesos)

@app.route('/api/data/todos_procesos', methods=['GET'])
def get_llamadas_todos_procesos():
    t_procesos = get_todos_procesos()
    return jsonify(t_procesos)

if __name__ == '__main__':
    app.run(host='localhost', port=5200)
```

## Dashboard
 En este archivo el cual pertenece al programacion en react, aqui tendremos las siugientes funciones:
* fetchData y fetchproc  
Tendremos las funciones para poder obtener os datos de la API el cual seran llevados posteriormente a las tablas.
* useEffect  
 En esta funcion podremos llamar a fetchData y a fetchProc para montar el componente cada 5 segundos.

Tambien tendremos las siguientes constantes que serviran para almacenar informacion y utilzarla posteriormente para diversas configuraciones.
* data  
Esta constante almacenara la informacion obtenida de la api en la direccion "http://localhost:5200/api/data/llamadas"
* proc  
Esta constante almacenara la informacion obtenida de la api en la direccion http://localhost:5200/api/data/procesos"
* opciones  
Esta constante tendra las configruraciones necesarias para el correcto funcionamiento de la grafica de pie, de esta forma se podra visualizar y estara colocada de la manera correcta en el dashboard.
* restantePorcentaje  
Esta constante calculara la parte retante para llegar al 100%
* pieData  
Esta constante servira para almacenar los datos de la grafica de pie.
## Otros
Aqui se estara manejando la tabla Otros, la cual poseera la informacion de todos los procesos retornados por el systemtap, estos estaran en la ruta 'http://localhost:5200/api/data/todos_procesos', para este manejaremos las siguientes funciones.
* useEffect  
aqui llamaremos a fech Data cada 5 segundos para actualizar su informacion.
* fetchProc  
en esta funcion podremos obtener los datos de la Api.

## Navbar
Aqui estara el contenido que mostrara el navbar del dashboard, este estara utilizando la libreria "styled-components" para su manejo en css.
## Main
Aqui se estaran manejando las direcciones del dashboard, este empreara la libreria "react-router-dom" el cual servira para poder direccionar estos, utilizando la propiedad ReactDOM estos podran mostrarse en el dashboard.  
* Home  
Esta funcion tendra la direccion Navbar el cual manejara las rutas del dashboard.
* Dashboard  
Esta funcion tendra la direccion navbar y el codigo del dashboard manejado en el archivo dashboard.jsx.
* Otross  
Esta funcion estara manejando la direccion del Navbar y de Otros el cual manejara el archivo Otros.jsx.
