#include "pila.h"
#include <stdlib.h>
#include <stdio.h>

/* Definición del struct pila proporcionado por la cátedra.
 */

 struct pila{
     void **datos;
     size_t tam;
     size_t largo;
 };

const size_t LARGO_INICIAL=10;
const size_t FACTOR_REDIMENSION = 2;
enum redimensiones {TRUNCAR, EXPANDIR};
/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...

pila_t* pila_crear(){
    // damian: que pasa si esto falla?
    pila_t* pila = malloc(sizeof(pila_t));
    if(!pila){
        return NULL;
    }
    pila->datos=malloc(LARGO_INICIAL*sizeof(void*));
    if(pila->datos==NULL){
        free(pila);
        return NULL;
    }
    pila->largo=0;
    pila->tam=LARGO_INICIAL*sizeof(void*);
    return pila;
}
void pila_destruir(pila_t* pila){
    free(pila->datos);
    free(pila);
}
//redimensiona el arreglo de punteros de a cuerdo a las opciones
//TRUNCAR o EXPANDIR. Si no se redimensionó correctamente
//devuelve false, en caso contrario devuelve true
bool redimensionar(pila_t* pila,int redimension){
    void* nuevos_datos = NULL;
    size_t tamanio=0;
    if(redimension==TRUNCAR){
        // damian: el factor de redimension deberia ser una constante
        tamanio= pila->tam/FACTOR_REDIMENSION;
    }
    else if(redimension==EXPANDIR){
        // damian: para que los parentesis
        tamanio = FACTOR_REDIMENSION*pila->tam;
    }
    nuevos_datos=realloc(pila->datos,tamanio);
    if(nuevos_datos!=NULL){
        pila->tam=tamanio;
        pila->datos=nuevos_datos;
        return true;
    }
    return nuevos_datos;
}
bool pila_esta_vacia(const pila_t *pila){
    return pila->largo==0;

}
bool pila_apilar(pila_t* pila, void* valor){
    if((pila->largo)*sizeof(void*) == pila->tam){
        if(!redimensionar(pila, EXPANDIR)){
            return false;
        }
    }
    pila->datos[pila->largo]=valor;
    pila->largo++;
    return true;
}
void* pila_ver_tope(const pila_t *pila){
    if(pila->largo==0){
        return NULL;
    }
    return pila->datos[(pila->largo)-1];
}
void* pila_desapilar(pila_t *pila){
    if(pila->largo == 0){
        return NULL;
    }
    size_t tamanio_usado=(pila->largo)*sizeof(void*);
    if (tamanio_usado<=(pila->tam)/2 && pila->tam > LARGO_INICIAL*sizeof(void*)){
        // damian: que pasa si esta redimension falla? no se esta manejando ese caso y es algo que puede pasar
        if(!redimensionar(pila,TRUNCAR)){
            return false;
        }
    }
    (pila->largo)--;
    return pila->datos[pila->largo];
}
