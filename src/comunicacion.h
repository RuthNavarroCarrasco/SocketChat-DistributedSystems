#ifndef COMUNICACION_H
#define COMUNICACION_H

#define MAXSIZE 255


// Usamos esto para evitar problema de redefinición

struct tupla_pet {
    int clave;              /* clave */
    char valor1[MAXSIZE];   /* valor1 */
    int valor2;             /* valor2 */
    double valor3;          /* valor3 */
};



// Usamos esto para evitar problema de redefinición

struct peticion {
    struct tupla_pet tupla_peticion;     /* tupla de la peticion */
    int clave2;                         //OPCIONAL (solo para COPY)
    char q_name[MAXSIZE];               /* nombre de la cola cliente */
    int c_op;                           /* codigo de operacion, 0 (INIT), 1 (SET), 2 (GET), 3 (MOD), 4 (DEL), 5 (EXIST), 6 (COPY) */
};



// Usamos esto para evitar problema de redefinición

struct respuesta {
    struct tupla_pet tupla_peticion;    /* tupla de la peticion */
    int code_error;                     // código de error. 0 en caso de éxito, -1 en caso de error
};

#endif 
