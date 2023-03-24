#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "comunicacion.h"
#include "claves.h"

#define peticion_root "../peticion/" // raiz para coger los ficheros
#define formato_fichero ".dat"      // definimos el formato de fichero. En este caso, extension .dat

#define SERVIDOR "/SERVIDOR"

int send_recieve(struct peticion *peticion) {
    int cod, sd_server;
    struct sockaddr_in address;
    struct respuesta respuesta;

    sd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (sd_server < 0) 
    {
        perror("Socket client: ");
        return -1;
    }

    // Nos conectamos al servidor
    cod = connect(sd_server, (struct sockaddr *)&address, sizeof(address));

    if (ret < 0) 
    {
        perror("connect to server: ");
        return -1;
    }
    


    // Enviamos la petición
    if (write(sd_server, (int) peticion.tupla_peticion.clave, sizeof(int)) < 0)
    {
        perror("write: ");
        return -1;
    }

    if (write(sd_server, (char *) peticion.tupla_peticion.valor1, sizeof(char)) < 0)
    {
        perror("write: ");
        return -1;
    }


    if (write(sd_server, (int) peticion.tupla_peticion.valor2, sizeof(int)) < 0)
    {
        perror("write: ");
        return -1;
    }


    // Podemos mandarlo como una cadena de texto porque no se pueden pasar 64 bits, solo 32.
    if (write(sd_server, (double) peticion.tupla_peticion.valor3, sizeof(double)) < 0)
    {
        perror("write: ");
        return -1;
    }


    if (write(sd_server, (int) peticion.clave2, sizeof(int)) < 0)
    {
        perror("write: ");
        return -1;
    }


    if (write(sd_server, (int) peticion.c_op, sizeof(int)) < 0)
    {
        perror("write: ");
        return -1;
    }



    // recibimos la respuesta
    if (read(sd_server, (int) respuesta.tupla_peticion.clave, sizeof(int)) < 0)
    {
        perror("read respuesta: ");
        return -1;
    }

    if (read(sd_server, (char *) respuesta.tupla_peticion.valor1, sizeof(char)) < 0)
    {
        perror("read respuesta: ");
        return -1;
    }


    if (read(sd_server, (int) respuesta.tupla_peticion.valor2, sizeof(int)) < 0)
    {
        perror("read respuesta: ");
        return -1;
    }


    if (read(sd_server, (double) respuesta.tupla_peticion.valor3, sizeof(double)) < 0)
    {
        perror("read respuesta: ");
        return -1;
    }


    if (read(sd_server, (int) respuesta.code_error, sizeof(int)) < 0)
    {
        perror("read respuesta: ");
        return -1;
    }

    return respuesta.code_error;
}



int init() {

    //creamos la peticion
    struct peticion peticion = {0};
    peticion.c_op = 0;

    sprintf(peticion.q_name, "/cliente_%d", getpid());

    int code_error = send_recieve(&peticion);
    return code_error;
}

int set_value(int key, char *value1, int value2, double value3) {
    int code_error;
    //creamos la peticion 
    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    strcpy(peticion.tupla_peticion.valor1, value1);
    peticion.tupla_peticion.valor2 = value2;
    peticion.tupla_peticion.valor3 = value3;
    
   // sprintf(peticion.q_name, "/cliente_%d", getpid());
    
    peticion.c_op = 1;
    code_error = send_recieve(&peticion);

    return code_error;
}


int get_value(int key, char *value1, int *value2, double *value3) {
    //creamos la peticion 
    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    
    peticion.c_op = 2;

    sprintf(peticion.q_name, "/cliente_%d", getpid());
    
    int code_error = send_recieve(&peticion );
    
    strcpy(value1, peticion.tupla_peticion.valor1);
    *value2 = peticion.tupla_peticion.valor2;
    *value3 = peticion.tupla_peticion.valor3;
    
    return code_error;
}


int modify_value(int key, char *value1, int value2, double value3){
    //creamos la peticion 
    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    strcpy(peticion.tupla_peticion.valor1, value1);
    peticion.tupla_peticion.valor2 = value2;
    peticion.tupla_peticion.valor3 = value3;
    peticion.c_op = 3;
    
    sprintf(peticion.q_name, "/cliente_%d", getpid());

    int code_error = send_recieve(&peticion);

    return code_error;
}


int delete_key(int id) {

    //creamos la peticion 
    struct peticion peticion = {
        .tupla_peticion.clave = id,
        .c_op = 4
    };
    
    sprintf(peticion.q_name, "/cliente_%d", getpid());

    int code_error = send_recieve(&peticion);

    return code_error;
}



int exist_key(int id) {

    //creamos la peticion 
    struct peticion peticion = {
        .tupla_peticion.clave = id,
        .tupla_peticion.valor2 = 0,
        .c_op = 5
    };

    sprintf(peticion.q_name, "/cliente_%d", getpid());

    int code_error = send_recieve(&peticion);

    return code_error;
}


int copy_key(int key1, int key2) {  

    //creamos la peticion 
    struct peticion peticion = {
        .tupla_peticion.clave = key1,
        .clave2 = key2,
        .c_op = 6
    };
    
    sprintf(peticion.q_name, "/cliente_%d", getpid());

    int code_error = send_recieve(&peticion);

    return code_error;
}
