CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
LDFLAGS = -L. -lrt

CLIENT = cliente.c
LIB_FILES = claves.c
LIB = libclaves.so
LIB_FLAGS_COMP = -c -fPIC
LIB_FLAGS_LINK = -shared -o
LIB_OBJS = $(LIB_FILES:.c=.o)

SERVER = servidor.c implementacion.c

all: cliente servidor
	 rm ../tuplas/*
	 export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.


cliente: $(CLIENT) $(LIB)
	$(CC) $(DFLAGS) cliente.c -lclaves -o cliente $(LDFLAGS)


$(LIB): $(LIB_FILES)
	$(CC) $(LIB_FLAGS_COMP) $(DFLAGS) $(LIB_FILES)
	$(CC) $(LIB_FLAGS_LINK) $(LIB) $(LIB_OBJS) -lrt

servidor: $(SERVER)
	$(CC) $(DFLAGS) $(SERVER) -lpthread -lrt -o servidor

clean:
	rm -f cliente servidor $(LIB) $(LIB_OBJS)
