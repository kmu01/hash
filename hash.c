#include <stdlib.h>
#include <stdlib.h>
#include "hash.h"
#include "lista.h"

// ********** Definiciones **********

typedef struct nodo{
	char* clave;
	void* dato;
} nodo_t;

struct hash{
	lista_t** listas;
	size_t tam:
	size_t cant;
	hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
	lista_iter_t* lista_iter;
	size_t indice_actual;
	const hash_t* hash;
};

// ********** Auxiliares **********

// Función de hashing, la versión mas simple es generar la clave segun el string, pero también es estupidamente ineficiente.
// La idea es usar una de las 2 primeras listadas en:: www.cse.yorku.ca/~oz/hash.html
size_t f_hash(char* str){
 	/* hola






	







	le erre el test








 	que tal */
}

