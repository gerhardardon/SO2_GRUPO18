#ifndef READ_JSON_H
#define READ_JSON_H
struct data_struct
{ //  creamos una estructura para guardar los datos de los usuarios
    int no_cuenta;
    char nombre[50];
    float saldo;
};
void read_json_file(char* filename);
void reporte_estado_cuenta();
extern int usuarios_size;
extern struct data_struct usuarios[200];

#endif
