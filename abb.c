#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "pila.h"

struct _nodo
{
	char *clave;
	void *dato;
};

typedef struct _nodo nodo_t;

struct abb
{
	nodo_t* raiz;
	struct abb *rama_izq, *rama_der;
	abb_comparar_clave_t f_comparacion;
	abb_destruir_dato_t f_destrucion;
	size_t cant_elementos;
};

struct abb_iter
{
	pila_t* pila_inorder;
};

/* establecemos que f_comparacion devuelve 1 si es mayor
 * y -1 si el dato es menor */

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato)
{
	abb_t* arbol = malloc( sizeof(abb_t) );
	if( !arbol )
		return NULL;
	nodo_t* raiz = malloc(sizeof(nodo_t));
	if(!raiz){
		free(arbol);
		return NULL;
	}
	arbol->raiz=raiz;
	arbol->raiz->clave = NULL;
	arbol->raiz->dato = NULL;
	arbol->f_comparacion = cmp;
	arbol->f_destrucion = destruir_dato;
	arbol->rama_izq = NULL;
	 arbol->rama_der = NULL;
	arbol->cant_elementos = 0;

	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato)
{
	char* copia_clave;
	copia_clave = strdup(clave);
	if( !arbol->cant_elementos ) /* primer elemento que agregamos?*/
	{
		arbol->raiz->clave = copia_clave;
		arbol->raiz->dato = dato;
	}
	int comparacion=arbol->f_comparacion(clave, arbol->raiz->clave);
	if(comparacion > 0 )
	{
		if(arbol->rama_izq==NULL){
			abb_t* nuevo_hijo= abb_crear(arbol->f_comparacion,arbol->f_destrucion);
			nuevo_hijo->cant_elementos=1;
			nuevo_hijo->raiz->clave=copia_clave;
			nuevo_hijo->raiz->dato=dato;
			arbol->rama_izq = nuevo_hijo;
		}
		else{
			abb_guardar(arbol->rama_izq, copia_clave, dato);
		}

	}
	else if( comparacion < 0 )
	{
		if(arbol->rama_der==NULL){
			abb_t* nuevo_hijo= abb_crear(arbol->f_comparacion,arbol->f_destrucion);
			nuevo_hijo->cant_elementos=1;
			nuevo_hijo->raiz->clave=copia_clave;
			nuevo_hijo->raiz->dato=dato;
			arbol->rama_der = nuevo_hijo;
		}
		else{
			abb_guardar(arbol->rama_der, copia_clave, dato);
		}
	}
	arbol->cant_elementos++;
	return true;
}


void swap (void** x, void** y) {
	void* direccion_y = *y;
	*y = *x;
	*x = direccion_y;
}

void imprimir_cad(const char cadena[]){
    int actual=0;
    while(cadena[actual]!='\0'){
        printf("%c",cadena[actual]);
        actual++;
    }
    printf("\n");
}
void* abb_borrar_recursivo(abb_t* arbol, const char* clave,pila_t* camino_recorrido){
	void *dato;
	if(!arbol){
		return NULL;
	}
	int comparacion=arbol->f_comparacion(clave, arbol->raiz->clave);
	if( comparacion >  0 )
	{
	//	printf("izq\n");
		pila_apilar(camino_recorrido,arbol);
		dato=abb_borrar_recursivo(arbol->rama_izq, clave,camino_recorrido);
		pila_desapilar(camino_recorrido);
		if(dato){
			arbol->cant_elementos--;
		}
	}
	else if( comparacion < 0 )
	{
	//	printf("der\n");
		pila_apilar(camino_recorrido,arbol);
		dato=abb_borrar_recursivo(arbol->rama_der, clave,camino_recorrido);
		pila_desapilar(camino_recorrido);
		if(dato){
			arbol->cant_elementos--;
		}
	}
	else{
	 //caso en que este es el nodo que corresponde borrar

		if( !arbol->rama_izq && !arbol->rama_der ){
			printf("1ro\n");
			// Arbol no tiene ningun hijo izquierdo ni derecho
			dato = arbol->raiz->dato;
			if(!pila_esta_vacia(camino_recorrido)){
				abb_t* padre= pila_desapilar(camino_recorrido);
				if(padre->rama_izq==arbol){
					padre->rama_izq=NULL;
				}
				else if(padre->rama_der==arbol){
					padre->rama_der=NULL;
				}
				free(arbol->raiz->clave);
				free(arbol->raiz);
				free(arbol);
			}
			else{
				//caso en que el nodo es la raiz
				free(arbol->raiz->clave);
				//free(arbol->raiz);
				//arbol->raiz=NULL;
				arbol->raiz->dato=NULL;
				arbol->cant_elementos=0;

			}
		}
		else if( (!arbol->rama_izq && arbol->rama_der) || (arbol->rama_izq && !arbol->rama_der) )
		{		printf("2do\n");

			// tiene un hijo, necesitamos reemplazar
			// si falta la parte derecha, reemplazamos por el izquierdo
			// Si falta la parte izquierda, reemplazamos por la derecha
			//abb_t* arbol_temporal = (!arbol->rama_der) ? arbol->rama_izq : arbol->rama_der;
			dato = arbol->raiz->dato;
			abb_t* nodo_a_elmiminar=arbol;
			abb_t* siguiente;
			if(arbol->rama_der){
				siguiente=arbol->rama_der;
			}
			else{
				siguiente=arbol->rama_izq;
			}
			if(!pila_esta_vacia(camino_recorrido)){
				printf("tiene p\n");
				abb_t* padre=pila_desapilar(camino_recorrido);
				if(padre->rama_izq==arbol){
					padre->rama_izq=siguiente;
				}
				else if(padre->rama_der==arbol){
					padre->rama_der=siguiente;
				}
			}
			else{
				printf("no tiene p\n");
				arbol=siguiente;
			}

			free(nodo_a_elmiminar->raiz->clave);
			free(nodo_a_elmiminar->raiz);
			free(nodo_a_elmiminar);
		}
		else
		{		printf("3ro\n");

			abb_t* anterior = arbol->rama_izq;
			while( anterior->rama_der != NULL ){
				anterior = anterior->rama_der;
			}
			const char* clave_raiz = arbol->raiz->clave;
			dato=arbol->raiz->dato;
			 swap((void**)&anterior,(void**)&arbol);
			abb_borrar(anterior,clave_raiz);
		}

	}
	return dato;

}

void *abb_borrar(abb_t *arbol, const char *clave)
{
	if ( !arbol || arbol->cant_elementos==0 ){
		return NULL;
	}
	pila_t* camino_recorrido=pila_crear();
	void* dato=abb_borrar_recursivo(arbol,clave,camino_recorrido);
	pila_destruir(camino_recorrido);
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave)
{
	if ( !arbol || arbol->cant_elementos==0 ){
		return NULL;
	}
	if( arbol->f_comparacion(clave, arbol->raiz->clave) > 0 )
		return abb_obtener(arbol->rama_izq, clave);
	else if( arbol->f_comparacion(clave, arbol->raiz->clave) < 0 )
		return abb_obtener(arbol->rama_der, clave);
	else
	{
		void *dato = arbol->raiz->dato;
		return dato;
	}
}

bool abb_pertenece(const abb_t *arbol, const char *clave)
{
	return abb_obtener(arbol, clave) != NULL;
}

size_t abb_cantidad(abb_t *arbol)
{
	return arbol->cant_elementos;
}

void abb_destruir(abb_t *arbol)
{
	if( !arbol ){
		return;
	}

	abb_destruir(arbol->rama_izq);
	abb_destruir(arbol->rama_der);

	if(arbol->raiz){
		free(arbol->raiz->clave);
		free(arbol->raiz);
		if(arbol->f_destrucion ){
			arbol->f_destrucion(arbol->raiz->dato);
		}
	}
	free(arbol);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra)
{
	if( arbol )
	{
		abb_in_order(arbol->rama_izq, visitar, extra);
		visitar(arbol->raiz->clave, arbol->raiz->dato, extra);
		abb_in_order(arbol->rama_der, visitar, extra);
	}
}
void abb_pre_order(abb_t *arbol, bool visitar(const char *, void *, void *),void* extra){
	if( arbol )
	{
		visitar(arbol->raiz->clave, arbol->raiz->dato, extra);
		abb_in_order(arbol->rama_izq, visitar, extra);
		abb_in_order(arbol->rama_der, visitar, extra);
	}
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol)
{
	abb_iter_t* iterador = malloc( sizeof(abb_iter_t) );

	if( !iterador )
		return NULL;

	iterador->pila_inorder = pila_crear();

	if( !iterador->pila_inorder )
	{
		free(iterador);
		return NULL;
	}
	if(arbol->cant_elementos){
		pila_apilar(iterador->pila_inorder, arbol->raiz);

		abb_t* tmp = arbol->rama_izq;

		/*apilamos todos los izquierdos*/
		while( tmp )
		{
			pila_apilar(iterador->pila_inorder, tmp);

			tmp = tmp->rama_izq;
		}
	}
	return iterador;
}
bool abb_iter_in_al_final(const abb_iter_t* iter){
	return pila_esta_vacia(iter->pila_inorder);
}
bool abb_iter_in_avanzar(abb_iter_t *iter)
{
	if(abb_iter_in_al_final(iter)){
		return false;
	}
	abb_t* desapilado = pila_desapilar(iter->pila_inorder);

	pila_apilar(iter->pila_inorder, desapilado->rama_der);

	abb_t* tmp = desapilado->rama_izq;

	while( tmp )
	{
		pila_apilar(iter->pila_inorder, tmp);
		tmp = tmp->rama_izq;
	}
	return true;
	/* mas condiciones */
}
const char *abb_iter_in_ver_actual(const abb_iter_t *iter)
{
	if( abb_iter_in_al_final(iter) || pila_esta_vacia(iter->pila_inorder) )
		return NULL;

	return pila_ver_tope(iter->pila_inorder);
}

void abb_iter_in_destruir(abb_iter_t* iter){

	free(iter);
}
