#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS				   *
 *******************************************************************/

typedef struct nodo{
	void* dato;
	struct nodo* prox;
}nodo_t;

struct lista{
	struct nodo* prim;
	struct nodo* ult;
	size_t largo;
};

struct lista_iter{
	nodo_t* actual;
	nodo_t* ant;
	lista_t* lista;
};

nodo_t *nodo_crear(void *dato){
	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if (nuevo == NULL){
		return NULL;
	}
	nuevo->dato = dato;
	return nuevo;
}

/*******************************************************************
 *                    PRIMITIVAS DE LA PILA						   *
 *******************************************************************/

lista_t *lista_crear(void){
	lista_t *lista = malloc(sizeof(lista_t));
	if (lista == NULL){
		return NULL;
	}
	lista->ult = NULL;
	lista->prim = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t* lista){
	return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t *nuevo = nodo_crear(dato);
	if (nuevo == NULL){
		return false;
	}
	
	if (lista_esta_vacia(lista)){
		lista->ult = nuevo;
		nuevo->prox = NULL;
	} else{
		nuevo->prox = lista->prim;
	}
	lista->prim = nuevo;
	lista->largo++;
	return true;
}


bool lista_insertar_ultimo(lista_t *lista, void *dato){
	if (lista_esta_vacia(lista)){
		return lista_insertar_primero(lista, dato);
	}

	nodo_t *nuevo = nodo_crear(dato);
	if (nuevo == NULL){
		return false;
	}
	nuevo->prox = NULL;
	lista->ult->prox = nuevo;
	lista->ult = nuevo; 
	lista->largo++;
	return true;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)){
		return false;		
	}
	return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t *lista){
	if (lista_esta_vacia(lista)){
		return false;		
	}
	return lista->ult->dato;
}
 
void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	void *valor = lista->prim->dato;
	nodo_t *nodo_aux = lista->prim;
	lista->prim = lista->prim->prox;
	free(nodo_aux);
	lista->largo--;
	if (lista_esta_vacia(lista)){
		lista->ult = NULL;	
	}
	return valor;
} 

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	while (!lista_esta_vacia(lista)){
		void *dato = lista_borrar_primero(lista);
		if (destruir_dato){
			destruir_dato(dato);
		}
	}
	free(lista);
}

/*******************************************************************
 *               	   PRIMITIVAS DEL ITERADOR					   *
 *******************************************************************/

//ITERADOR INTERNO:

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if (lista_esta_vacia(lista) || visitar == NULL){
		return;
	}
	nodo_t *nodo = lista->prim;
	while(nodo != NULL && visitar(nodo->dato, extra) == true){
		nodo = nodo->prox;
	}
}

//ITERADOR EXTERNO:

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t *iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL){
		return NULL;
	}
	iter->lista = lista;
	iter->actual = lista->prim;
	iter->ant = NULL;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if (!iter->actual){
		return false;
	}
	iter->ant = iter->actual;
	iter->actual = iter->actual->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (!iter->actual){
		return NULL;
	}
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return (iter->actual == NULL);
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	// Si esta en la primer o ultima posición simplemente usa las primitivas de la lista.
	
	if (lista_iter_al_final(iter)){  
		if (!lista_insertar_ultimo(iter->lista, dato)){
			return false;
		}
		iter->actual = iter->lista->ult;
		if (lista_largo(iter->lista) == 1){
			iter->actual = iter->lista->prim;
		}
		return true;
	}

	nodo_t *nuevo = nodo_crear(dato);
	if (nuevo == NULL){
		return false;
	}
	
	nuevo->prox = iter->actual;	
	if (iter->actual == iter->lista->prim){
		iter->lista->prim = nuevo;
	} else{
		iter->ant->prox = nuevo;
	}
	iter->actual = nuevo;
	iter->lista->largo++;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
	
	if (lista_iter_al_final(iter)){
		return NULL;
	}

	nodo_t *nodo = iter->actual;
	void *valor = nodo->dato;
	
	if (iter->actual == iter->lista->prim){
		iter->lista->prim = nodo->prox;
		iter->actual = iter->lista->prim;
	}
	else if(iter->actual == iter->lista->ult){
		iter->lista->ult = iter->ant;
		iter->actual = NULL;
		iter->ant->prox = iter->actual;
	}
	else{
		iter->actual = nodo->prox;
		iter->ant->prox = iter->actual;
	}	
	iter->lista->largo--;
	free(nodo);
	return valor;
}


