from flask import Flask, jsonify
from flask_cors import CORS
import mysql.connector
from mysql.connector import Error

app = Flask(__name__)
CORS(app)  # Habilitar CORS en toda la aplicación

# Configuración de la base de datos
db_config = {
    'user': 'root',
    'password': 'root',
    'host': 'localhost',
    'database': 'proyecto2'
}

def get_solicitudes():
    # Conectar a la base de datos
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    print("Conectado a la base de datos")

    # Ejecutar la consulta
    cursor.execute("SELECT * FROM solicitudes")

    # Obtener todos los resultados
    solicitudes = cursor.fetchall()

    # Cerrar la conexión
    cursor.close()
    conn.close()

    return solicitudes


# esta funcion se encarga de generar los procesos haciendo query al tabala de solicitudes 
def generar_procesos():
    # Conectar a la base de datos
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)

    try:
        cursor = conn.cursor()

        # Limpiar todos los valores de la tabla 'procesos'
        cursor.execute("DELETE FROM procesos;")
        conn.commit()
        print("Tabla 'procesos' limpiada exitosamente")

        # Consultar las últimas llamadas mmap y munmap para cada pid
        query = (
            "WITH LastMmap AS ("
            "    SELECT pid, nombre, llamada, tamano, fecha "
            "    FROM solicitudes "
            "    WHERE llamada = 'mmap' "
            "    AND fecha = ("
            "        SELECT MAX(fecha) "
            "        FROM solicitudes "
            "        WHERE pid = solicitudes.pid AND llamada = 'mmap'"
            "    )"
            "), "
            "LastMunmap AS ("
            "    SELECT pid, nombre, llamada, tamano, fecha "
            "    FROM solicitudes "
            "    WHERE llamada = 'munmap' "
            "    AND fecha = ("
            "        SELECT MAX(fecha) "
            "        FROM solicitudes "
            "        WHERE pid = solicitudes.pid AND llamada = 'munmap'"
            "    )"
            ") "
            "SELECT m.pid, m.nombre, m.tamano AS tamano_mmap, n.tamano AS tamano_munmap "
            "FROM LastMmap m "
            "JOIN LastMunmap n ON m.pid = n.pid;"
        )
        
        cursor.execute(query)
        results = cursor.fetchall()

        for row in results:
            pid = row[0]
            nombre = row[1]
            tamano_mmap = row[2]
            tamano_munmap = row[3]
            memoriamb =  tamano_mmap - tamano_munmap 
            #memoria total en bytes
            memoria_total = 5905580032
            porcentaje_memoria = memoriamb / memoria_total
            if porcentaje_memoria < 0:porcentaje_memoria = 0
            if memoriamb < 0:memoriamb = 0
            
            insert_query = (
                "INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) "
                "VALUES (%s, %s, %s, %s) "
                "ON DUPLICATE KEY UPDATE "
                "nombre = VALUES(nombre), "
                "memoriamb = VALUES(memoriamb), "
                "porcentaje_memoria = VALUES(porcentaje_memoria);"
            )
            data = (pid, nombre, memoriamb, porcentaje_memoria)
            
            cursor.execute(insert_query, data)
        
        conn.commit()
        print("Procesos generados exitosamente")
        return True

    except Error as e:
        print(e)
    finally:
        cursor.close()
        conn.close()

def get_procesos():
    # Conectar a la base de datos
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)

    # Ejecutar la consulta
    cursor.execute("SELECT * FROM procesos ORDER BY pid DESC LIMIT 10;")

    # Obtener todos los resultados
    processo = cursor.fetchall()

    # Cerrar la conexión
    cursor.close()
    conn.close()

    return processo

def get_todos_procesos():
    # Conectar a la base de datos
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)

    # Ejecutar la consulta
    cursor.execute("SELECT * FROM procesos;")

    # Obtener todos los resultados
    t_processo = cursor.fetchall()

    # Cerrar la conexión
    cursor.close()
    conn.close()

    return t_processo

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