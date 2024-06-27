#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define QUERY_SIZE 4096

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

//ejecutar query
void execute_query(MYSQL *conn, const char *query) {
    if (mysql_query(conn, query)) {
        fprintf(stderr, "QUERY FAILED: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);

    }
}

int main() {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char query[QUERY_SIZE];

    //conectarse a db 
    MYSQL *conn = connect_to_database();
    fprintf(stderr, "Connected to database\n");

    //borrar contenido de la tabla
    execute_query(conn, "delete from solicitudes;");
    execute_query(conn, "delete from procesos;");
    fprintf(stderr, "-tablas limpiada con exito :)\n");

    fp = popen("sudo ./systap.stp", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run SystemTap script\n");
        exit(EXIT_FAILURE);
    }

    //leemos el systap.stp
    while (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {

        int pid, length;
        char execname[BUFFER_SIZE], action[BUFFER_SIZE], timestamp[4096];

        sscanf(buffer, "%d %s %s %d %s\n", &pid, execname, action, &length, timestamp);
        int n = snprintf(query, sizeof(query),
                         "insert into solicitudes (pid, nombre, llamada, tamano, fecha) values (%d, '%s', '%s', %d, '%s')",
                         pid, execname, action, length, timestamp);

        // Check for truncation
        if (n >= sizeof(query)) {
            fprintf(stderr, "Truncated query: %s\n", query);
            continue;
        }

        // Execute the query
        execute_query(conn, query);
        fprintf(stderr, "-insertado\n");
        usleep(500000);
        

    }

    // Close the SystemTap script
    pclose(fp);

    // Close the database connection
    mysql_close(conn);

    return 0;
}

// gcc lector.c -o lector -lmysqlclient