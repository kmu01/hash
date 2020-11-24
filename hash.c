#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// ********** Definiciones **********

#define TAM_DEFAULT 101
#define NO_BORRAR false
#define BORRAR true

typedef struct nodo_hash{
	char* clave;
	void* dato;
} nodo_hash_t;

struct hash{
	lista_t** listas;
	size_t tam;
	size_t cant;
	hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
	lista_iter_t* lista_iter;
	size_t actual;
	const hash_t* hash;
};

// ********** Auxiliares **********

// Función de hashing, la versión mas simple es generar la clave segun el string, pero también es estupidamente ineficiente.
// La idea es usar una de las 2 primeras listadas en:: www.cse.yorku.ca/~oz/hash.html
size_t f_hash(unsigned char *str){
	size_t hash = 5381;
    int c;
    while (c == *str++){
        hash = ((hash << 5) + hash) + c;   //hash * 33 + c
	}
	return hash;
}

static bool ubicar_listas(lista_t* lista){
	bool no_error = true;
	for(size_t i = 0; i < TAM_DEFAULT;i++){
		lista_t* lista_a_insertar = lista_crear();
		if(!lista_a_insertar){
			no_error = false;
		}
		lista_insertar_ultimo(lista,lista_a_insertar);
	}
	return no_error;
}

static nodo_hash_t* nodo_hash_crear(const char* clave, void* dato){
	nodo_hash_t* nodo = malloc(sizeof(nodo_hash_t));
	if(!nodo){
		return NULL;
	}
	nodo->clave = clave;
	nodo->dato = dato;
	return nodo;
}

//Busca un nodo dentro de la tabla hash.
nodo_hash_t* buscar_nodo(hash_t *hash, const char *clave, bool borrar){
	size_t pos = f_hash(clave) % hash->tam;
	if (lista_esta_vacia(hash->listas[pos])){
		return NULL;
	}
	lista_iter_t* iter_lista = lista_iter_crear(hash->listas[pos]);
	if (!iter_lista){
		return NULL;
	}
	nodo_hash_t* nodo = lista_iter_ver_actual(iter_lista);
	while (!lista_iter_al_final(iter_lista) && strcmp(nodo->clave, clave) != 0){
		nodo = lista_iter_ver_actual(iter_lista);
	}
	if (borrar && nodo){
		lista_iter_borrar(iter_lista);
	}
	lista_iter_destruir(iter_lista);
	return nodo;	
}



// ********** Primitivas **********

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash){
		return NULL;
	}
	hash->destruir_dato = destruir_dato;
	hash->tam = TAM_DEFAULT;
	hash->cant = 0;
	hash->listas = malloc(sizeof(lista_t*) * TAM_DEFAULT);
	if(!hash->listas){
		return NULL;
	}
	if(!ubicar_listas(hash->listas)){
		return NULL;
	}
	return hash;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	nodo_hash_t* nodo = buscar_nodo(hash,clave,NO_BORRAR);
	if(nodo){
		nodo->dato = dato;
		return true;
	}
	nodo = nodo_hash_crear(clave,dato);
	if(!nodo){
		return false;
	}
	size_t pos = f_hash(clave) % hash->tam;
	lista_insertar_ultimo(hash->listas[pos],nodo);
	hash->cant++;
	return true;
}

/* Borra un elemento del hash y devuelve el dato asociado. Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave){
	nodo_hash_t* nodo = buscar_nodo(hash, clave, BORRAR);
	if (!nodo) return NULL;
	void* dato = nodo->dato;
	hash->cant--;
	free(nodo);
	return dato;
}
/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave){
	return (nodo_hash_t*)buscar_nodo(hash, clave, NO_BORRAR)->clave;
}


/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave){
	return buscar_nodo(hash, clave, NO_BORRAR) != NULL;
}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash){
	return hash->cant;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash){
	size_t cont = 0;
	for(size_t i = 0; i < hash->tam;i++){
		if(lista_esta_vacia(hash->listas[cont])){
			lista_destruir(hash->listas[cont],NULL);
		} else{
		lista_destruir(hash->listas[cont], hash->destruir_dato);
		}
		cont++;
	}
	free(hash->listas);
	free(hash);
}


/* Iterador del hash */


// auxiliares

bool iter_buscar_ocupado(hash_iter_t *iter){
	while (iter->actual < iter->hash->tam){
		if (!lista_esta_vacia(iter->hash->listas[iter->actual])){
			iter->lista_iter->hash->listas[iter->actual];
			iter->lista_iter->lista->actual = 0;
			iter->lista_iter->lista->ant = NULL;
			return true;
		}
		iter->actual++;
	}
	//Si no encuentra nada, simplemente se queda al final.
	iter->lista_iter = NULL;
	return true;
}

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter){
		return NULL;
	}
	iter->actual = 0;
	iter->hash = hash;
	iter->lista_iter = lista_iter_crear(iter->hash->listas[0]);
	if(!iter->lista_iter){
		return NULL;
	}
	return iter;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)){
		return false;
	} 
	lista_iter_avanzar(iter->lista_iter);
	if (!lista_iter_al_final(iter->lista_iter)){
		return true;
	}
	return iter_buscar_ocupado(iter);
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)){
		return NULL;
	} 
	nodo_hash_t* nodo = lista_iter_ver_actual(iter->lista_iter);
	return nodo->clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter){
	//Si el actual es igual al tamaño, se recorrieron todas las posiciones.
	return iter->actual == iter->hash->tam;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t *iter){
	if (iter->lista_iter) {
		lista_iter_destruir(iter->lista_iter);
	}
	free(iter);
}