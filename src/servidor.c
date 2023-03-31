#include <sys/socket.h>
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

#define SERVIDOR "/SERVIDOR"
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
    

    printf("\n\nEl código de operación ARRIBA ES es: %d\n", mensaje.c_op);
   // printf("El valor1 es: %s\n", mensaje.tupla_peticion.valor1);
    

    //leemos y ejecutamos la petición

    if (mensaje.c_op == 0) //init
        resultado = init_implementacion();

   if (mensaje.c_op  == 1) //set
        resultado = set_value_implementacion(mensaje.tupla_peticion);

     if (mensaje.c_op == 2) //get 
        resultado = get_value_implementacion(mensaje.tupla_peticion, &respuesta); 

    if (mensaje.c_op  == 3) //mod
        resultado = modify_value_implementacion(mensaje.tupla_peticion);

    if (mensaje.c_op == 4) //del
        resultado = delete_key_implementacion(mensaje.tupla_peticion.clave);

    if (mensaje.c_op  == 5) //exist
       resultado = exist_key_implementacion(mensaje.tupla_peticion.clave);

    if (mensaje.c_op  == 6) //copy
       resultado = copy_key_implementacion(mensaje.tupla_peticion.clave, mensaje.clave2);
    
    respuesta.code_error = resultado;

    
    respuesta.code_error = htonl(respuesta.code_error);
    respuesta.tupla_peticion.clave = htonl(respuesta.tupla_peticion.clave);
    printf("La clave que voy a enviar es: %d\n ", respuesta.tupla_peticion.clave);
    printf("El valor1 que voy a enviar es: %s\n ", respuesta.tupla_peticion.valor1);
    printf("El valor2 que voy a enviar es: %d\n ", respuesta.tupla_peticion.valor2);
    printf("El valor3 que voy a enviar es: %lf\n ", respuesta.tupla_peticion.valor3);
    printf("El cod error que voy a enviar es: %d\n ", respuesta.code_error);

    //unsigned long int value1 = strtoul(respuesta.tupla_peticion.valor1, NULL, 0);
    //value1 = htonl(value1);
    respuesta.tupla_peticion.valor2 = htonl(respuesta.tupla_peticion.valor2);
    
    //se abre el socket del cliente y se envian todos los campos de la respuesta

    if (write ( ((struct peticion *)mess)->sd_client, &respuesta.code_error, sizeof(int)) < 0)
    {
        perror("write: ");
    }
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

    //unsigned long int value3 = strtoul(valor3, NULL, 0);
    //value3 = htonl(value3);

    if (write(((struct peticion *)mess)->sd_client, &valor3, sizeof(valor3)) < 0)
    {
        perror("write: ");
    }


    close (((struct peticion *)mess)->sd_client);


	pthread_exit(0);
}

int main(void){
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



     // open server sockets
     if ((sd_server = socket(AF_INET, SOCK_STREAM, 0)) == 0)
     {
        perror("socket: ");
        exit(-1);
     }


    // socket options
    if (setsockopt(sd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
     {
         perror("setsockopt: ");
         exit(-1);
     }


    // bind + listen
     address.sin_family      = AF_INET ;
     address.sin_addr.s_addr = INADDR_ANY ;
     address.sin_port        = htons(4200) ;

     

     if (bind(sd_server, (struct sockaddr *)&address,  sizeof(address)) < 0)
     {
         perror("bind: ");
         exit(-1);
     }

     if (listen(sd_server, 3) < 0)
     {
         perror("listen: ") ;
         exit(-1);
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
		exit(-1);
	    }
        
        //se reciben todos los campos de la petición del cliente
        read ( sd_client, (char*) &clave, sizeof(int));
        read ( sd_client, (char*) &valor1, sizeof(valor1));
        read ( sd_client, (char*) &valor2, sizeof(int));
        read ( sd_client, (char*) &valor3, sizeof(valor3));
        read ( sd_client, (char*) &clave2, sizeof(int));
        read ( sd_client, (char*) &c_op, sizeof(int));
      
        
        
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