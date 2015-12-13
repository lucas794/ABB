#include "cola.h"
#include <stdlib.h>


/*Definicion de los nodos de la cola*/

struct nodo_cola{
    void* dato;
    struct nodo_cola* siguiente;

};
typedef struct nodo_cola nodo_t;
/* Definicion de estructura cola */
struct cola{
    nodo_t* primero;
        nodo_t* ultimo;
};


//Funciones para crear y destruir los nodos
nodo_t* nodo_crear(void* dato){
    nodo_t* nodo=malloc(sizeof(nodo_t));
    if(nodo==NULL){
        return nodo;
    }
    nodo->dato = dato;
    nodo->siguiente=NULL;
    return nodo;
}
//Pre: el dato ya debio ser liberado
void nodo_destruir(nodo_t* nodo){
    free(nodo);
}

bool cola_esta_vacia(const cola_t* cola){
    return cola->primero==NULL;
}


cola_t* cola_crear(void){
    cola_t* cola=malloc(sizeof(cola_t));
    if(!cola){
        return NULL;
    }
    cola->primero=NULL;
    cola->ultimo=NULL;
    return cola;
}

void cola_destruir(cola_t* cola,void (*destruir_dato)(void*) ){

    nodo_t* actual=cola->primero;
    while(actual!=NULL){
        nodo_t* nodo = actual;
        if(destruir_dato != NULL){
            destruir_dato(actual->dato);
        }
        actual=actual->siguiente;
        nodo_destruir(nodo);
    }
    free(cola);
}


bool cola_encolar(cola_t* cola,void* dato){
    nodo_t* nodo = nodo_crear(dato);
    if(nodo==NULL){
        return false;
    }

    if(cola_esta_vacia(cola)){
        cola->primero=nodo;
    }
    else{
        cola->ultimo->siguiente=nodo;
    }
    cola->ultimo=nodo;
    return true;
}
void* cola_ver_primero(const cola_t* cola){
    if(cola_esta_vacia(cola)){
        return NULL;
    }
    return cola->primero->dato;
}
void* cola_desencolar(cola_t* cola){
    if(cola_esta_vacia(cola)){
        return NULL;
    }
    void* dato= cola->primero->dato;
    nodo_t* primero = cola->primero;
    cola->primero=cola->primero->siguiente;
    if(!cola->primero){
        cola->ultimo=NULL;
    }
    nodo_destruir(primero);
    return dato;
}
