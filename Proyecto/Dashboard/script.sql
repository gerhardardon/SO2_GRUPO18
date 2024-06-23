CREATE DATABASE proyecto2;
USE proyecto2;
CREATE TABLE solicitudes (
    pid INT PRIMARY KEY,
    nombre VARCHAR(50),
    llamada VARCHAR(50),
    tamano INT,
    fecha DATETIME
);
INSERT INTO solicitudes (pid, nombre, llamada, tamano, fecha) VALUES (2087, 'Juan Pérez', 'mmap', 100, '2023-05-15 14:30:00');
INSERT INTO solicitudes (pid, nombre, llamada, tamano, fecha) VALUES (2088, 'Cristian Gomez', 'mmap', 100, '2023-05-01 13:22:00');
INSERT INTO solicitudes (pid, nombre, llamada, tamano, fecha) VALUES (2089, 'Hector Martinez', 'mmap', 324, '2023-05-01 22:05:00');

select * from solicitudes;

TRUNCATE TABLE solicitudes;



CREATE TABLE procesos (
    pid INT PRIMARY KEY, 
    nombre VARCHAR(50),
    memoriamb FLOAT,
    porcentaje_memoria FLOAT
);

INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2087, 'Chrome', 750, 0.15);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2088, 'Firefox', 542.2, 0.24);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2089, 'Edge', 324.5, 0.12);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2090, 'Opera', 100.5, 0.05);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2091, 'Safari', 200.5, 0.10);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2092, 'Brave', 150.5, 0.07);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2093, 'Vivaldi', 300.5, 0.15);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2094, 'Chromium', 400.5, 0.20);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2095, 'Tor', 500.5, 0.25);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2096, 'Epic', 600.5, 0.30);
INSERT INTO procesos (pid, nombre, memoriamb, porcentaje_memoria) VALUES (2097, 'Waterfox', 700.5, 0.01);

-- seleccionamos los primeros 10 registros de la tabla procesos
SELECT * FROM procesos LIMIT 10;
-- seleccionamos los ultimos 10 registros de la tabla procesos
SELECT * FROM procesos ORDER BY pid DESC LIMIT 10;