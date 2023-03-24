#ifndef CLAVES_H
#define CLAVES_H
// Declaraciones de las funciones.

#include "comunicacion.h"

int send_recieve(struct peticion *peticion);

int init();

int set_value(int key, char *value1, int value2, double value3);

int get_value(int key, char *value1, int *value2, double *value3);

int modify_value(int key, char *value1, int value2, double value3);

int delete_key(int key);

int exist_key(int key);

int copy_key(int key1, int key2);

#endif 