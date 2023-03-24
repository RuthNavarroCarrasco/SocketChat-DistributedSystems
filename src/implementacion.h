#ifndef CLAVES_H
#define CLAVES_H
/* Servicios*/
#include "comunicacion.h"

int init_implementacion();

int set_value_implementacion(struct tupla_pet peticion);

int get_value_implementacion(struct tupla_pet peticion, struct respuesta *respuesta);

int modify_value_implementacion(struct tupla_pet peticion);

int delete_key_implementacion(int key);

int exist_key_implementacion(int key);

int copy_key_implementacion(int key1, int key2);

#endif 