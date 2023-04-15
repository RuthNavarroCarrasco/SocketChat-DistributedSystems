#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "implementacion.h"
#define peticion_root "../tuplas/" // raiz para coger los ficheros
#define formato_fichero ".dat"      // definimos el formato de fichero. En este caso, extension .dat


int init_implementacion()
{   
    //Esta función borra todos los ficheros que representan las claves en el directorio peticion_root
    DIR *theFolder = opendir(peticion_root);
    struct dirent *next_file;
    
    char filepath[400];
    
    while ( (next_file = readdir(theFolder)) != NULL )
    {
        // build the path for each file in the folder
        sprintf(filepath, "%s/%s", "../tuplas/", next_file->d_name);
        remove(filepath);
    }
    closedir(theFolder);
    
    

    return 0;
}


int set_value_implementacion(struct tupla_pet peticion) 
{
    FILE *fichero_peticion;
    char str_key[20];
    char nombre_fichero[50];

    sprintf(str_key, "%d", peticion.clave);
    sprintf(nombre_fichero, "%s%s%s", peticion_root, str_key, formato_fichero);
    
    // Comprobamos la existencia de la clave

    if (access(nombre_fichero, F_OK) == 0) 
    {
        perror("set_value(): La clave existe");
        return -1;
    }

    fichero_peticion = fopen(nombre_fichero, "wb");

    if (fichero_peticion != NULL) 
    {
        fwrite(&peticion, sizeof(struct tupla_pet), 1, fichero_peticion); 
        fclose(fichero_peticion);  
    } 
    else 
    {
        perror("set_value(): No se pudo abrir el archivo.\n");
        return -1;
    }

   return 0;
}

int get_value_implementacion(struct tupla_pet tupla, struct respuesta *respuesta)
{
    //Esta función devuelve un struct que representa los valores de la clave key
    char str_key[20];
    char nombre_fichero[50];

    sprintf(str_key, "%d", tupla.clave);
    sprintf(nombre_fichero, "%s%s%s", peticion_root, str_key, formato_fichero);

    // Comprobamos la existencia de la clave
    if (access(nombre_fichero, F_OK) != 0) 
    {
        perror("get_value(): La clave no existe");
        return -1;
    }

   FILE *archivo = fopen(nombre_fichero, "rb");  // Abrir el archivo para lectura binaria
    if (archivo != NULL) 
    {
        fread(&(respuesta->tupla_peticion), sizeof(struct tupla_pet), 1, archivo);  // Leer la estructura desde el archivo
        fclose(archivo);  // Cerrar el archivo
    } 
    else 
    {
        printf("get_value(): No se pudo abrir el archivo.\n");
        return -1;
    }
    
    return 0;
}



int modify_value_implementacion(struct tupla_pet peticion) 
{
    //Esta función modifica el fichero que representa la clave key con los nuevos valores

    char str_key[20];
    char nombre_fichero[50];
    struct tupla_pet pet;

    sprintf(str_key, "%d", peticion.clave);
    sprintf(nombre_fichero, "%s%s%s", peticion_root, str_key, formato_fichero);

    // Comprobamos la existencia de la clave
    if (access(nombre_fichero, F_OK) != 0) 
    {
        perror("modify_value(): La clave no existe");
        return -1;
    }

    FILE *archivo = fopen(nombre_fichero, "r+b");
    
    if (archivo == NULL) 
    {
        perror("Modify_value_impl(): Error al abrir el archivo\n");
        return -1;
    }

    // Mover el puntero de posición al inicio del archivo
    fseek(archivo, 0, SEEK_SET);

    // Leer el registro original
    struct tupla_pet peticion_original;
    fread(&peticion_original, sizeof(struct tupla_pet), 1, archivo);

    // Mover el puntero de posición al inicio del archivo
    fseek(archivo, 0, SEEK_SET);

    // Escribir el nuevo registro
    fwrite(&peticion, sizeof(struct tupla_pet), 1, archivo);

    fclose(archivo);

    return 0;

}

int delete_key_implementacion(int key)
{
    char str_key[20];
    char nombre_fichero[50];

    sprintf(str_key, "%d", key);
    sprintf(nombre_fichero, "%s%s%s", peticion_root, str_key, formato_fichero);

    // Comprobamos si hay existencia del fichero
    if (access(nombre_fichero, F_OK) != 0) 
    {
        perror("delete_key_implementacion(): La clave no existe");
        return -1;
    }

    //eliminamos el fichero
    if (remove(nombre_fichero) != 0) 
    {
        perror("Error al eliminar el fichero");
        return -1;
    } 
    printf("Fichero eliminado correctamente");

    
    return 0;

}

int exist_key_implementacion(int key)
{
    char str_key[20];
    char nombre_fichero[50];

    sprintf(str_key, "%d", key);
    sprintf(nombre_fichero, "%s%s%s", peticion_root, str_key, formato_fichero);

    // Comprobamos si no hay existencia del fichero
    if (access(nombre_fichero, F_OK) != 0) 
    {
        perror("exist_key(): El fichero no existe\n");
        return -1;
    }
    
    return 0;
}

int copy_key_implementacion(int key1, int key2){
    
    char str_key[20];
    char str_key2[20];
    char nombre_fichero[50];
    char nombre_fichero2[50];
    struct tupla_pet peticion_copiada;

    sprintf(str_key, "%d", key1);
    sprintf(str_key2, "%d", key2);
    sprintf(nombre_fichero, "%s%s%s", peticion_root, str_key, formato_fichero);
    sprintf(nombre_fichero2, "%s%s%s", peticion_root, str_key2, formato_fichero);

    //comprobamos si key1 existe
    if (access(nombre_fichero, F_OK) != 0) 
    {
        perror("copy_key(): El fichero no existe\n");
        return -1;
    }

    FILE *archivo = fopen(nombre_fichero, "rb");  // Abrir el archivo para lectura binaria
    if (archivo != NULL) 
    {
        fread(&peticion_copiada, sizeof(struct tupla_pet), 1, archivo);  // Leer la estructura desde el archivo
        fclose(archivo);  // Cerrar el archivo

    } else {
        perror("copy_key");
        return -1;
    }
    peticion_copiada.clave = key2;

    //si key2 existese modifica

    if (access(nombre_fichero2, F_OK) == 0) {
        if (modify_value_implementacion(peticion_copiada) < 0) {
            perror("No se ha podido copiar los valores\n");
            return -1;
        } else {
            return 0;
        }
        
    }

    //si key2 no existe se crea
    else{
         
        if (set_value_implementacion(peticion_copiada) < 0) {
            perror("No se ha podido copiar los valores\n");
            return -1;
        }
    }

  
    return 0;
}

