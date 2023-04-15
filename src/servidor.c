#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "comunicacion.h"
#include "implementacion.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXSIZE 255



//mutex
pthread_mutex_t mutex_mensaje;
int mensaje_no_copiado;
pthread_cond_t cond_mensaje;

void tratar_mensaje(void *mess) 
{

    struct peticion mensaje;	        //mensaje local 
    struct respuesta respuesta;	        // respuesta local 		          
    int resultado;		                // resultado de la operación 

    pthread_mutex_lock(&mutex_mensaje);

    //copia la petición a la variable mensaje
    mensaje = (*(struct peticion *) mess);
    
    //Como ya se ha copiado el mensaje, despetarmos al servidor 
    mensaje_no_copiado = false;
    pthread_cond_signal(&cond_mensaje);
	pthread_mutex_unlock(&mutex_mensaje);


    //leemos y ejecutamos la petición

    switch(mensaje.c_op) {
    case 0:
        resultado = init_implementacion();
        break;
    case 1:
        resultado = set_value_implementacion(mensaje.tupla_peticion);
        break;
    case 2:
        resultado = get_value_implementacion(mensaje.tupla_peticion, &respuesta);
        break;
    case 3:
        resultado = modify_value_implementacion(mensaje.tupla_peticion);
        break;
    case 4:
        resultado = delete_key_implementacion(mensaje.tupla_peticion.clave);
        break;
    case 5:
        resultado = exist_key_implementacion(mensaje.tupla_peticion.clave);
        break;
    case 6:
        resultado = copy_key_implementacion(mensaje.tupla_peticion.clave, mensaje.clave2);
        break;
    default:
        printf("Error: código de operación no válido.\n");
        exit(-1);
        break;
}

    
    respuesta.code_error = resultado;
    respuesta.code_error = htonl(respuesta.code_error);
    respuesta.tupla_peticion.clave = htonl(respuesta.tupla_peticion.clave);
    respuesta.tupla_peticion.valor2 = htonl(respuesta.tupla_peticion.valor2);
    
    // Se envian todos los campos de la respuesta

    if (write ( ((struct peticion *)mess)->sd_client, &respuesta.tupla_peticion.clave, sizeof(int)) < 0)
    {
        perror("write: ");
    }
    if (write ( ((struct peticion *)mess)->sd_client, &respuesta.tupla_peticion.valor1, sizeof(respuesta.tupla_peticion.valor1)) < 0)
    {
        perror("write: ");  
    }
    if (write ( ((struct peticion *)mess)->sd_client, &respuesta.tupla_peticion.valor2, sizeof(int)) < 0)
    {
        perror("write: ");  
    }
    
    char valor3[100];
    sprintf(valor3, "%lf", respuesta.tupla_peticion.valor3);


    if (write(((struct peticion *)mess)->sd_client, &valor3, sizeof(valor3)) < 0)
    {
        perror("write: ");
    }

    if (write ( ((struct peticion *)mess)->sd_client, &respuesta.code_error, sizeof(int)) < 0)
    {
        perror("write: ");
    }
    
    close (((struct peticion *)mess)->sd_client);


	pthread_exit(0);
}


int main(int argc, char *argv[]){
    struct peticion mess;
	pthread_attr_t t_attr;		
   	pthread_t thid;
    
    int clave;
    unsigned long int value1, value3;
    char valor1[MAXSIZE];
    int valor2;
    char valor3[100];
    int clave2;
    int c_op;

    struct sockaddr_in address; // direccion del servidor
    int sd_server; // socket del servidor
    int sd_client; // socket del cliente
    int addrlen = sizeof(address); // longitud de la direccion
    int opt = 1; // opcion para el setsockopt
    int port_number;

    if (argc < 2) 
    {
        printf("Pon el segundo argumento por favor te lo pido\n");
        return -1;
    } else {
        port_number = atoi(argv[1]);
    }

     // open server sockets
     if ((sd_server = socket(AF_INET, SOCK_STREAM, 0)) == 0)
     {
        perror("socket: ");
        return -1;
     }

    // socket options
    if (setsockopt(sd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
     {
         perror("setsockopt: ");
         return -1;
     }


    // bind + listen
     address.sin_family      = AF_INET ;
     address.sin_addr.s_addr = INADDR_ANY ;
     address.sin_port        = htons(port_number) ;


    if (bind(sd_server, (struct sockaddr *)&address,  sizeof(address)) < 0)
    {
        perror("bind: ");
        return -1;
    }

    if (listen(sd_server, 3) < 0)
    {
        perror("listen: ") ;
        return -1;
    }


	pthread_mutex_init(&mutex_mensaje, NULL);
	pthread_cond_init(&cond_mensaje, NULL);
	pthread_attr_init(&t_attr);

	// atributos de los threads, threads independientes que no necesitan hacer un join al final
	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);
    mensaje_no_copiado = true;

    while(1) {
        
        // se establece la conexión con el cliente
        sd_client = accept(sd_server, (struct sockaddr *)&address,  (socklen_t*)&addrlen) ;
       	if (sd_client <= 0)
	     {
		    perror("accept");
		    return -1;
	    }
        

        // Se reciben todos los campos de la petición del cliente
        if (read(sd_client, (char*) &clave, sizeof(int)) < 0) {
            perror("Error al leer clave");
            return -1;
        }

        if (read(sd_client, (char*) &valor1, sizeof(valor1)) < 0) {
            perror("Error al leer valor1");
            return -1;
        }

        if (read(sd_client, (char*) &valor2, sizeof(int)) < 0) {
            perror("Error al leer valor2");
            return -1;
        }

        if (read(sd_client, (char*) &valor3, sizeof(valor3)) < 0) {
            perror("Error al leer valor3");
            return -1;
        }

        if (read(sd_client, (char*) &clave2, sizeof(int)) < 0) 
        {
            perror("Error al leer clave2");
            return -1;
        }

        if (read(sd_client, (char*) &c_op, sizeof(int)) < 0) {
            perror("Error al leer c_op");
            return -1;
        }

      
        
        clave = ntohl(clave);
        valor2 = ntohl(valor2);
        clave2 = ntohl(clave2);
        c_op = ntohl(c_op);


        //se rellena la estructura de la petición
        mess.tupla_peticion.clave = clave;
        strcpy(mess.tupla_peticion.valor1, valor1);
        mess.tupla_peticion.valor2 = valor2;
        mess.tupla_peticion.valor3 = atof(valor3);
        mess.clave2 = clave2;
        mess.sd_client = sd_client;
        mess.c_op = c_op;
    

        if (pthread_create(&thid, &t_attr, (void *)tratar_mensaje, (void *)&mess) == 0) {
            // se espera a que el thread copie el mensaje 
			pthread_mutex_lock(&mutex_mensaje);
			while (mensaje_no_copiado)
				pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
			mensaje_no_copiado = true;
			pthread_mutex_unlock(&mutex_mensaje);
        }
			
    }

    return 0;
    
}