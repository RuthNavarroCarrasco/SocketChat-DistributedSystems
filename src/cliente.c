#define MAXSIZE 255
#include <stdio.h>
#include <stdlib.h>
#include "claves.h"

/*El cliente se encarga de enviar peticiones al servidor */

int clave = 10;              
char valor1[MAXSIZE] = "Enviando";   
int valor2 = 4;             
double valor3 = 1.72;
int clave2 = 102;


int main() 
{
    int code_error;

    
    code_error = set_value(clave, valor1, valor2, valor3);
    if (code_error < 0) 
    {
        printf("set_value(): código de error %d\n", code_error);
    } else {
        printf("set_value(): Se ha creado correctamente la clave %d. Código de error: %d\n\n",clave, code_error);
    }



    
    code_error = get_value(clave, valor1, &valor2, &valor3);
    if (code_error < 0) 
    {
        printf("get_value(): código de error %d\n", code_error);
    } else {
        printf("get_value(): Se ha procesado correctamente la petición. Los valores obtenidos de la tupla con clave %d son: %s, %d y %f\n\n", clave, valor1, valor2, valor3);
    }

    

    code_error = modify_value(clave, "Modificación", 37, 7.95);
    if (code_error < 0) 
    {
        printf("modify_value(): código de error %d\n", code_error);
    } else {
        printf("modify_value(): Se ha modificado correctamente la tupla. Código de error: %d\n\n", code_error);
    }
    
    code_error = exist_key(clave);
    if (code_error < 0) 
    {
        printf("exist_key(): código de error %d\n", code_error);
    }
    else{
        printf("exist_key(): El fichero sí existe. Código de error: %d\n\n", code_error);
    }

    
    code_error = copy_key(clave, clave2);
    if (code_error < 0) 
    {
        printf("copy_key(): código de error %d\n", code_error);
    }
    else{
        printf("copy_key(): Se ha copiado correctamente los valores de la clave %d a la clave %d. Código de error: %d\n\n", clave, clave2, code_error);
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