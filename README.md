# ejercicio_evaluable2

## Compilación del programa

En este programa tenemos dos ejecutables: servidor y cliente. Para ello, vamos a necesitar hacer uso de dos terminales para poder ejecutar el programa.

#### Terminal 1

En la **terminal 1** será donde ejecutemos el `servidor`. Para ello, deberemos poner las siguientes lineas de comando:
```C
make
./servidor <port>
```

Donde <port> es el número de puerto que queremos que use el servidor.

#### Terminal 2

En la **terminal 2** será donde ejecutemos el `cliente`. El cliente debe ser ejecutado una vez se haya lanzado el servidor, ya que de no ser así, el cliente estará haciendo peticiones pero no habrá servidor que pueda procesarlas. En el lado del servidor hacemos uso de las variables de entorno para evitar tener una dirección físicamente programada en el código y no tener que pasar la IP y el puerto en la línea de mandatos del programa cliente. Estas variables de entorno son: *IP_TUPLAS* y *PORT_TUPLAS*. Para definir el valor de estas variables de entorno al programa cliente, podemos pasarlo de la siguiente manera:

```C
env IP_TUPLAS=localhost PORT_TUPLAS=<port> ./cliente
```

Donde <port> es el mismo puerto que hemos puesto en el lado del cliente, Cabe aclarar que si no es así, no podrá haber comunicación entre el servidor y el cliente, ya que ambos tendrán números de puerto distintos. 

