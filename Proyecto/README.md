
# Manual tecnico
|Nombre  | Carnet | 
|------------- | -------------|
| Cristian Daniel Gomez Escobar |202107190 |
| Gerhard Benjamin Ardon Valdez  |202004796 |
# Descripcion del Proyecto
La gestión de la memoria es un aspecto crítico del rendimiento del sistema, ya que garantiza la utilización eficiente de los recursos y mantiene la estabilidad bajo diferentes cargas de trabajo. Al monitorear exhaustivamente la memoria, se puede obtener información valiosa sobre el comportamiento de su
sistema, identificar posibles cuellos de botella y optimizar la asignación de recursos. En este proyecto se creara una aplicación capaz de monitorear el uso de memoria de cada proceso abierto en Linux, detectando las solicitudes de memoria que estos realizan al sistema operativo.
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
