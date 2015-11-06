#define _XOPEN_SOURCE 500
#include "testing.h"
#include <stdlib.h>
#include "abb.h"
#include <stdio.h>
#include <string.h>
void imprimir_cadena(const char cadena[]){
    int actual=0;
    while(cadena[actual]!='\0'){
        printf("%c",cadena[actual]);
        actual++;
    }
    printf("\n");
}
bool visitar(const char* cadena, void* dato , void* extra){
    imprimir_cadena(cadena);
    return true;
}
bool guardar_preorder(const char* cadena, void* dato, void* array){
    int* dato_entero = dato;
    char** arreglo_claves=array;
    char* clave = strdup(cadena);
    arreglo_claves[*dato_entero]=clave;
    return true;
}
void prueba_abb_vacio(void){
    printf("ABB VACIO:\n");
    abb_t* arbol = abb_crear(strcmp,NULL);
    print_test("Arbol vacío...",abb_cantidad(arbol)==0);
    print_test("Borrar...",abb_borrar(arbol,"cadena")==NULL);
    print_test("Obtener...",abb_obtener(arbol,"cadena")==NULL);
    print_test("Pertenece...",abb_pertenece(arbol,"cadena")==false);

    printf("ITERADOR:\n");
    abb_iter_t* iter=abb_iter_in_crear(arbol);
    print_test("Iterador actual es NULL...",abb_iter_in_ver_actual(iter)==NULL);
    print_test("Iterador al final...",abb_iter_in_al_final(iter)==true);
    print_test("No se puede avanzar...",abb_iter_in_avanzar(iter)==false);
    abb_iter_in_destruir(iter);
    abb_destruir(arbol);

}
void prueba_abb_guardar_borrar(void){
    printf("GUARDAR Y BORRAR:\n");
    printf("GUARDAR:\n");
    int elementos[] = {0,1,2,3,4,5,6,7,8,9};
    char* claves[] = {"cero","uno","dos","tres","cuatro","cinco","seis",
                    "siete","ocho","nueve"};
    abb_t* arbol = abb_crear(strcmp,NULL);
    for(int i=0;i<10;i++){
        print_test("Guardar entero...",abb_guardar(arbol,claves[i],&elementos[i]));
    }
    print_test("Longitud correcta...",abb_cantidad(arbol)==10);

    for(int i=0;i<10;i++){
        print_test("Elemento guardado pertenece...",abb_pertenece(arbol,claves[i]));
        print_test("Obtener elemento...",abb_obtener(arbol,claves[i])==&elementos[i]);
    }
    printf("BORRAR HOJAS:\n");
    for(int i=9;i>=0;i--){
        print_test("Borrar...",abb_borrar(arbol,claves[i])==&elementos[i]);
        print_test("Longitud correcta...",abb_cantidad(arbol)==i);

    }
    printf("GUARDAR:\n");
    for(int i=0;i<10;i++){
        print_test("Guardar entero...",abb_guardar(arbol,claves[i],&elementos[i]));
    }
    print_test("Longitud correcta...",abb_cantidad(arbol)==10);

    for(int i=0;i<10;i++){
        print_test("Elemento guardado pertenece...",abb_pertenece(arbol,claves[i]));
        print_test("Obtener elemento...",abb_obtener(arbol,claves[i])==&elementos[i]);
    }
    printf("BORRAR NODOS INTERNOS:\n");
    char* preorder[10];
    abb_pre_order(arbol,guardar_preorder,preorder);
    for(int i=0;i<=1;i++){
        imprimir_cadena(preorder[i]);
        print_test("Borrar...",abb_borrar(arbol,preorder[i])==&elementos[i]);
        print_test("Longitud correcta...",abb_cantidad(arbol)==i);

    }/*
    printf("GUARDAR:\n");
    for(int i=0;i<10;i++){
        print_test("Guardar entero...",abb_guardar(arbol,claves[i],&elementos[i]));
    }
    print_test("Longitud correcta...",abb_cantidad(arbol)==10);

    for(int i=0;i<10;i++){
        print_test("Elemento guardado pertenece...",abb_pertenece(arbol,claves[i]));
        print_test("Obtener elemento...",abb_obtener(arbol,claves[i])==&elementos[i]);
    }
    printf("BORRAR RAIZ:\n");
    for(int i=0;i<10;i++){
        print_test("Borrar...",abb_borrar(arbol,claves[i])==&elementos[i]);
        print_test("Longitud correcta...",abb_cantidad(arbol)==i);
    }*/
    abb_destruir(arbol);
}
