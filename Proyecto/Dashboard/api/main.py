from flask import Flask, jsonify
from flask_cors import CORS
import mysql.connector

app = Flask(__name__)
CORS(app)  # Habilitar CORS en toda la aplicación

# Configuración de la base de datos
db_config = {
    'user': 'root',
    'password': 'secret',
    'host': 'localhost',
    'port': 3306,
    'database': 'proyecto2'
}

def get_solicitudes():
    # Conectar a la base de datos
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)

    # Ejecutar la consulta
    cursor.execute("SELECT * FROM solicitudes")

    # Obtener todos los resultados
    solicitudes = cursor.fetchall()

    # Cerrar la conexión
    cursor.close()
    conn.close()

    return solicitudes

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
    cursor.execute("SELECT * FROM procesos")

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