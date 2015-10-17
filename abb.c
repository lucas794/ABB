#include <stdio.h>
#include <stdlib.h>
#include "abb.h"

struct _nodo
{
	const char *clave;
	void *dato;
};

typedef struct _nodo nodo_t;

struct abb
{
	nodo_t* vagon;
	struct abb *rama_izq, *rama_der;
	abb_comparar_clave_t f_comparacion;
	abb_destruir_dato_t f_destrucion;
	size_t cant_elementos;
};

struct abb_iter
{
	abb_t* actual;
};
/* establecemos que f_comparacion devuelve 1 si es mayor
 * y -1 si el dato es menor */

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato)
{
	abb_t* arbol = malloc( sizeof(abb_t) );

	if( !arbol )
		return NULL;

	nodo_t* tmp_info = malloc( sizeof(nodo_t) );
	
	if( !tmp_info )
	{
		free(arbol);
		return NULL;
	}
	arbol->vagon->clave = NULL;
	arbol->vagon->dato = NULL;
	arbol->f_comparacion = cmp;
	arbol->f_destrucion = destruir_dato;
	arbol->rama_izq = arbol->rama_der = NULL;
	arbol->cant_elementos = 0;
	
	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato)
{
	if( !arbol )
	{
		abb_t* tmp_arbolito = abb_crear(arbol->f_comparacion, arbol->f_destrucion);
		if( !tmp_arbolito ) return false;
		
		tmp_arbolito->vagon->clave = clave;
		tmp_arbolito->vagon->dato = dato;
	}
	else if( !arbol->cant_elementos ) /* primer elemento que agregamos?*/
	{
		arbol->vagon->clave = clave;
		arbol->vagon->dato = dato;
		arbol->cant_elementos = 1;
	}
	else if( arbol->f_comparacion(clave, arbol->vagon->clave) < 0 )
	{
		// recursion D:
		abb_guardar(arbol->rama_izq, clave, dato);
		
		// ponemos esto luego de la recursion 
		// para evitar el caso que la recursion falle!
		arbol->cant_elementos = arbol->rama_izq->cant_elementos + 1;
	}
	else if( arbol->f_comparacion(clave, arbol->vagon->clave) > 0 )
	{
		abb_guardar(arbol->rama_der, clave, dato);
		arbol->cant_elementos = arbol->rama_der->cant_elementos + 1;
	}
	
	return true;
}

void *abb_borrar(abb_t *arbol, const char *clave)
{
	if ( !arbol ) /* caso base */
		return NULL;
		
	if( arbol->f_comparacion(clave, arbol->vagon->clave) < 0 )
	{
		abb_borrar(arbol->rama_izq, clave);
		arbol->cant_elementos = arbol->rama_der->cant_elementos - 1;
		// ¿No hay que descontar la cantidad de hijos que tenia ?
	}
	else if( arbol->f_comparacion(clave, arbol->vagon->clave) > 0 )
	{
		abb_borrar(arbol->rama_der, clave);
		arbol->cant_elementos = arbol->rama_der->cant_elementos - 1;
		// aplica lo mismo que puse arriba ! :(
	}
	else
	{
		void *dato;
		// lo hemos encontrado !! :D
		if( !arbol->rama_izq && !arbol->rama_der )
		{
			// Arbol no tiene ningun hijo izquierdo ni derecho
			dato = arbol->vagon->dato;
			if( arbol->f_destrucion )
			{
				arbol->f_destrucion(arbol->vagon->dato);
			}
			free(arbol->vagon);
			free(arbol);
			return dato;
		}
		else if( (!arbol->rama_izq && arbol->rama_der) || (arbol->rama_izq && !arbol->rama_der) )
		{
			// tiene un hijo, necesitamos reemplazar
			// si falta la parte derecha, reemplazamos por el izquierdo
			// Si falta la parte izquierda, reemplazamos por la derecha
			abb_t* arbol_temporal = (!arbol->rama_der) ? arbol->rama_izq : arbol->rama_der;
			dato = arbol->vagon->dato;
			
			if( arbol->f_destrucion )
			{
				arbol->f_destrucion(arbol->vagon->dato);
			}
			free(arbol->vagon);
			free(arbol);
			
			arbol = arbol_temporal;
		}
		else
		{
			// tiene los 2 hijos, cagué como un campeón
			// lo dejo para mas adelante porque tengo que pensarlo
			// deberia reemplazarlo por el dato mas grande del arbol izquierdo?
		}
	}
}

void *abb_obtener(const abb_t *arbol, const char *clave)
{
	if( !arbol )
		return NULL;
	
	if( arbol->f_comparacion(clave, arbol->vagon->clave) < 0 )
		return abb_obtener(arbol->rama_izq, clave);
	else if( arbol->f_comparacion(clave, arbol->vagon->clave) > 0 )
		return abb_obtener(arbol->rama_der, clave);
	else
	{
		// wii , lo encontramos !!
		void *dato = arbol->vagon->dato;
		return dato;
	}
}

bool abb_pertenece(const abb_t *arbol, const char *clave)
{
	return (abb_obtener(arbol, clave) != NULL) ? true : false;
}

size_t abb_cantidad(abb_t *arbol)
{
	return arbol->cant_elementos;
}

void abb_destruir(abb_t *arbol)
{
	if( !arbol )
		return;
		
	abb_destruir(arbol->rama_izq);
	abb_destruir(arbol->rama_der);
	
	if( !arbol->f_destrucion )
		arbol->f_destrucion(arbol->vagon->dato);

	free(arbol->vagon);
	free(arbol);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra)
{
	if( arbol )
	{
		abb_in_order(arbol->rama_izq, visitar, extra);
		visitar(arbol->vagon->clave, arbol->vagon->dato, extra); // Estará bien??
		abb_in_order(arbol->rama_der, visitar, extra);
	}
}

/*
 *   H
   /   \
  E     K
 / \   / \
A   F I   Q
* 
* */

// el iterador debe hacer este proceso.
//AEFHIKQ	

abb_iter_t *abb_iter_in_crear(const abb_t *arbol /*meepa que no es const acá*/)
{
	abb_iter_t* iterador = malloc( sizeof(abb_iter_t) );
	
	if( !iterador )
		return NULL;
		
	iterador->actual = arbol;
	
	if( !iterador->actual )
		return NULL;
		
	while( iterador->actual->rama_izq != NULL ) // empezamos desde el de mas a la izquierda
		iterador->actual = iterador->actual->rama_izq;
		
	return iterador;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter)
{
	if( 1 ) /* abb_iter_in_al_final vá pero no está escrita. */
		return NULL;
	
	return iterador->actual->vagon->clave;
}
	