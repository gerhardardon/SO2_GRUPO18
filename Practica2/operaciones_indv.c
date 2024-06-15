#include <stdio.h>
#include "read_json.h"
#include "operaciones_indv.h"
void depositos(int no_cuenta, float monto)
{
    for (int i = 0; i < usuarios_size; i++)
    {
        if (usuarios[i].no_cuenta == no_cuenta)
        {
            usuarios[i].saldo += monto;
            printf("Deposito exitoso\n");
            return;
        }
    }
    printf("No se encontro la cuenta\n");
}

void retiros(int no_cuenta, float monto)
{
    for (int i = 0; i < usuarios_size; i++)
    {
        if (usuarios[i].no_cuenta == no_cuenta)
        {
            if (usuarios[i].saldo >= monto)
            {
                usuarios[i].saldo -= monto;
                printf("Retiro exitoso\n");
            }
            else
            {
                printf("Saldo insuficiente\n");
            }
            return;
        }
    }
    printf("No se encontro la cuenta\n");
}

void transacciones(int no_cuenta_origen, int no_cuenta_destino, float monto)
{
    for (int i = 0; i < usuarios_size; i++)
    {
        if (usuarios[i].no_cuenta == no_cuenta_origen)
        {
            if (usuarios[i].saldo >= monto)
            {
                usuarios[i].saldo -= monto;
                for (int j = 0; j < usuarios_size; j++)
                {
                    if (usuarios[j].no_cuenta == no_cuenta_destino)
                    {
                        usuarios[j].saldo += monto;
                        printf("Transaccion exitosa\n");
                        return;
                    }
                }
                printf("No se encontro la cuenta destino\n");
                return;
            }
            else
            {
                printf("Saldo insuficiente\n");
                return;
            }
        }
    }
    printf("No se encontro la cuenta origen\n");
}

void consultar_cuenta(int no_cuenta)
{
    for (int i = 0; i < usuarios_size; i++)
    {
        if (usuarios[i].no_cuenta == no_cuenta)
        {
            printf("no_cuenta: %d   nombre: %s  saldo: %2f \n",
                   usuarios[i].no_cuenta, usuarios[i].nombre, usuarios[i].saldo);
            return;
        }
    }
    printf("No se encontro la cuenta\n");
}
