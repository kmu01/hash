#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS				   *
 *******************************************************************/

struct lista;
typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;


/*******************************************************************
 *                    PRIMITIVAS DE LA PILA						   *
 *******************************************************************/

// Crea una lista.
// Pos: Devuelve una lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista contiene elementos, falso en caso contrario.
// Pre: La lista fue creada.
bool lista_esta_vacia(const lista_t* lista);

// Inserta un nuevo elemento en el primer lugar de la lista. Devuelve falso en caso de error.
// Pre: La lista fue creada.
// Pos: Se agregó un nuevo elemento a la lista, ubicado al principio de esta.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un nuevo elemento en el ultimo lugar de la lista. Devuelve falso en caso de error.
// Pre: La lista fue creada.
// Pos: Se agregó un nuevo elemento a la lista, ubicado al final de esta.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Devuelve la cantidad de elementos en la lista.
// Pre: La lista fue creada.
size_t lista_largo(const lista_t *lista);

// Si tiene elementos, devuelve el valor del primer elemento de la lista.
// Si está vacía devuelve NULL.
// Pre: La lista fue creada.
// Post: Se devolvió el primer elemento de la lista, si no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Si tiene elementos, devuelve el valor del ultimo elemento de la lista.
// Si está vacía devuelve NULL.
// Pre: La lista fue creada.
// Post: Se devolvió el ultimo elemento de la lista, si no está vacía.
void *lista_ver_ultimo(const lista_t *lista);

// Si la lista tiene elementos, se quita el primero de la lista, y se devuelve su valor.
// Si está vacía, devuelve NULL.
// Pre: La lista fue creada.
// Post: Se devolvió el valor del primer elemento anterior, y la lista contiene un elemento menos.
void *lista_borrar_primero(lista_t *lista);


// Destruye la lista. 
// En caso de recibir la función destruir dato, esta se llama para cada uno de sus elementos.
// Pre: La lista fue creada. destruir_dato es una función capaz de destruir elementos.
// Post: Se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/*******************************************************************
 *               	   PRIMITIVAS DEL ITERADOR					   *
 *******************************************************************/

//ITERADOR INTERNO:

// Recorre la lista de principio a fin, aplicando la funcion visitar a cada elemento.
// Pre: La lista fue creada
// Post: La lista se iteró hasta el final o hasta que se cumplió la condición de corte de visitar.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

//ITERADOR EXTERNO:

// Crea un iterador
// Pre: La lista fue creada.
// Pos: Devuelve un iterador que comienza al principio de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posición en el iterador, si es posible.
// Pre: El iterador fue creado.
// Post: Se devolvió true si se pudo avanzar, y false si dicha posición no existe.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el elemento actual donde se encuentra el iterador.
// Pre: El iterador fue creado.
// Post: Se devolvió el elemento actual del iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Señala si se llegó al final de la lista.
// Pre: El iterador fue creado.
// Post: Se devolvió true si se esta al final, false si no.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: El iterador fue creado.
// Post: Se eliminó el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la posición actual de la lista.
// Pre: El iterador fueron creados.
// Post: Se añade un elemento en la posición actual de la lista.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento actual de la lista.
// Pre: El iterador fueron creados.
// Post: Se eliminó el elemento en la posición actual.
void *lista_iter_borrar(lista_iter_t *iter);

#endif  // LISTA_H