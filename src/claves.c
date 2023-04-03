#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "comunicacion.h"
#include "claves.h"

#define peticion_root "../peticion/" // raiz para coger los ficheros
#define formato_fichero ".dat"      // definimos el formato de fichero. En este caso, extension .dat


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


    address.sin_family = AF_INET ;
    address.sin_port   = htons(4200) ;

    cod = inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) ;

    if (cod <= 0) 
    { 
        printf("\nInvalid address or address not supported\n") ;
        close(sd_server) ;
        exit(-1);
    } 

    // Nos conectamos al servidor
    cod = connect(sd_server, (struct sockaddr *)&address, sizeof(address));
    
    if (cod < 0) 
    {
        perror("connect to server: ");
        return -1;
    }
    
    
    // Pasamos de double a string el valor3 (para poderlo tratar con sockets) porque no se pueden pasar 64 bits, solo 32.
    char valor3[100];
    sprintf(valor3, "%lf", peticion->tupla_peticion.valor3);
    

    peticion->tupla_peticion.clave = htonl(peticion->tupla_peticion.clave);
    peticion->tupla_peticion.valor2 = htonl(peticion->tupla_peticion.valor2);
    peticion->clave2 = htonl(peticion->clave2);
    peticion->c_op = htonl(peticion->c_op);



    // Enviamos la petición
    if (write(sd_server, (char *)&(peticion->tupla_peticion.clave), sizeof(int)) < 0)
    {
        perror("write: ");
        return -1;
    }

    if (write(sd_server,(char *) &peticion->tupla_peticion.valor1, sizeof(peticion->tupla_peticion.valor1)) < 0)
    {
        perror("write: ");
        return -1;
    }


    if (write(sd_server, &(peticion->tupla_peticion.valor2), sizeof(int)) < 0)
    {
        perror("write: ");
        return -1;
    }


   
    if (write(sd_server, (char *) &valor3, sizeof(valor3)) < 0)
    {
        perror("write: ");
        return -1;
    }


    if (write(sd_server, (char *) &(peticion->clave2), sizeof(int)) < 0)
    {
        perror("write: ");
        return -1;
    }


    if (write(sd_server, (char *) &(peticion->c_op), sizeof(int)) < 0)
    {
        perror("write: ");
        return -1;
    }



    // recibimos la respuesta
    if (read(sd_server, (char *) &(respuesta.tupla_peticion.clave), sizeof(int)) < 0)
    {
        perror("c read respuesta: ");
        return -1;
    }

    if (read(sd_server, &respuesta.tupla_peticion.valor1, sizeof(respuesta.tupla_peticion.valor1)) < 0)
    {
        perror(" 1 read respuesta: ");
        return -1;
    }


    if (read(sd_server, (char *) &(respuesta.tupla_peticion.valor2), sizeof(int)) < 0)
    {
        perror("2 read respuesta: ");
        return -1;
    }


    if (read(sd_server, (char *) &valor3, sizeof(valor3)) < 0)
    {
        perror("3 read respuesta: ");
        return -1;
    }

    

    if (read(sd_server, (char *) &(respuesta.code_error), sizeof(int)) < 0)
    {
        perror("4 read respuesta: ");
        return -1;
    }

    respuesta.tupla_peticion.clave = ntohl(respuesta.tupla_peticion.clave);
    respuesta.tupla_peticion.valor2 = ntohl(respuesta.tupla_peticion.valor2);
    respuesta.code_error = ntohl(respuesta.code_error);
    respuesta.tupla_peticion.valor3 = atof(valor3);
    
    // guardamos los valores de la resuesta para el caso en el que sea necesario devolverlos (f. get)
    strcpy(peticion->tupla_peticion.valor1, respuesta.tupla_peticion.valor1);
    peticion->tupla_peticion.valor2 = respuesta.tupla_peticion.valor2;
    peticion->tupla_peticion.valor3 = respuesta.tupla_peticion.valor3;
     
    return respuesta.code_error;
}



int init() {

    //creamos la peticion
    struct peticion peticion = {0};
    peticion.c_op = 0;
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
    
    peticion.c_op = 1;
    code_error = send_recieve(&peticion);

    return code_error;
}


int get_value(int key, char *value1, int *value2, double *value3) {
    //creamos la peticion 
    struct peticion peticion = {0};
    peticion.tupla_peticion.clave = key;
    peticion.c_op = 2;
    
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
    
    int code_error = send_recieve(&peticion);

    return code_error;
}


int delete_key(int id) {

    //creamos la peticion 
    struct peticion peticion = {
        .tupla_peticion.clave = id,
        .c_op = 4
    };
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
    
    int code_error = send_recieve(&peticion);

    return code_error;
}
