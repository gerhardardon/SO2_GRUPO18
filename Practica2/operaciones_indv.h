#ifndef OPERACIONES_INDV_H
#define OPERACIONES_INDV_H

void depositos(int no_cuenta, float monto);
void retiros(int no_cuenta, float monto);
void transacciones(int no_cuenta_origen, int no_cuenta_destino, float monto);
void consultar_cuenta(int no_cuenta);

#endif
