#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "root"
#define DB_NAME "proyecto2"

// Configuración de la base de datos
MYSQL *connect_db()
{
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    return conn;
}

void generar_procesos()
{
    MYSQL *conn = connect_db();
    MYSQL_RES *res;
    MYSQL_ROW row;

    // Limpiar todos los valores de la tabla 'procesos'
    if (mysql_query(conn, "DELETE FROM procesos;"))
    {
        fprintf(stderr, "DELETE FROM procesos failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    if (mysql_commit(conn))
    {
        fprintf(stderr, "Commit failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    printf("Tabla 'procesos' limpiada exitosamente\n");

    while (true)
    {

        // Consultar las últimas llamadas mmap y munmap para cada pid
        const char *query =
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
            "SELECT m.pid, m.nombre, m.tamano AS tamano_mmap, COALESCE(n.tamano, 0) AS tamano_munmap "
            "FROM LastMmap m "
            "LEFT JOIN LastMunmap n ON m.pid = n.pid;";

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "Query failed. Error: %s\n", mysql_error(conn));
            mysql_close(conn);
            exit(EXIT_FAILURE);
        }

        res = mysql_store_result(conn);
        if (res == NULL)
        {
            fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
            mysql_close(conn);
            exit(EXIT_FAILURE);
        }

        while ((row = mysql_fetch_row(res)))
        {
            int pid = atoi(row[0]);
            char *nombre = row[1];
            long tamano_mmap = atol(row[2]);
            long tamano_munmap = atol(row[3]);
            long memoriamb = tamano_mmap - tamano_munmap;
            long memoria_total = 5005580032;
            double porcentaje_memoria = (double)memoriamb / memoria_total;

            if (porcentaje_memoria < 0)
                porcentaje_memoria = 0;
            if (memoriamb < 0)
                memoriamb = 0;

            char insert_query[512];
            snprintf(insert_query, sizeof(insert_query),
                     "INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) "
                     "VALUES (%d, '%s', %ld, %f) "
                     "ON DUPLICATE KEY UPDATE "
                     "nombre = VALUES(nombre), "
                     "memoriamb = VALUES(memoriamb), "
                     "porcentaje_memoria = VALUES(porcentaje_memoria);",
                     pid, nombre, memoriamb, porcentaje_memoria);

            if (mysql_query(conn, insert_query))
            {
                fprintf(stderr, "Insert query failed. Error: %s\n", mysql_error(conn));
            }
        }

        if (mysql_commit(conn))
        {
            fprintf(stderr, "Commit failed. Error: %s\n", mysql_error(conn));
            mysql_free_result(res);
            mysql_close(conn);
            exit(EXIT_FAILURE);
        }

        printf("Procesos generados exitosamente\n");
        sleep(2);
    }
    mysql_free_result(res);
    mysql_close(conn);
}

int main()
{
    generar_procesos();
    return 0;
}
