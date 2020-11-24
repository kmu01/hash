#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hash.h"
#include "lista.h"

// ********** Definiciones **********

#define TAM_DEFAULT 101

typedef struct nodo{
	char* clave;
	void* dato;
} nodo_t;

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
    while (c = *str++){
        hash = hash * 33 + c;
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

//Busca un nodo dentro de la tabla hash.
nodo_t* buscar_nodo(const hash_t* hash; char* clave);


// ********** Primitivas **********

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash){
		return NULL;
	}
	hash->destruir_dato = destruir_dato;
	hash->tam = TAM_DEFAULT;
	hash->cant = 0;
	hash->listas = lista_crear();
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
bool hash_guardar(hash_t *hash, const char *clave, void *dato);

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave);

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave){

}


/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave);

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
	while(!lista_esta_vacia(hash->listas)){
		lista_destruir(hash->listas[cont], hash->destruir_dato);
		cont++;
	}
	lista_destruir(hash->listas,NULL);
	free(hash);
}


/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter){
	//Si el actual es igual al tamaño, se recorrieron todas las posiciones.
	return iter->actual == iter->hash->tam;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t *iter){
	free(iter);
}