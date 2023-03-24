#define MAXSIZE 255
#include <stdio.h>
#include <stdlib.h>
#include "claves.h"

/*El cliente se encarga de enviar peticiones al servidor */

int clave = 26;              
char valor1[MAXSIZE] = "Mensaje ";   
int valor2 = 3;             
double valor3 = 2.2;
int clave2 = 27;



int main() 
{
    int code_error;

    code_error = set_value(clave, valor1, valor2, valor3);
    if (code_error < 0) 
    {
        printf("set_value(): código de error %d\n", code_error);
    } else {
        printf("set_value(): Se ha creado correctamente la clave %d. Código de error: %d\n\n", clave,code_error);
    }

    code_error = delete_key(clave);
    if (code_error < 0) 
    {
        printf("delete_key(): código de error %d\n", code_error);
    }
    else{
        printf("detele_key(): Se ha eliminado correctamente la clave %d. Código de error: %d\n\n", clave, code_error);
    }

    

    code_error = init();
    if (code_error < 0) 
    {
        printf("init(): código de error %d\n", code_error);
    } else {
        printf("init(): Se han destruido correctametne todas las tuplas. Código de error: %d\n", code_error);
    }

    

    return 0; 
}