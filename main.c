#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include "pruebas_alumnos.h"

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

int main(int argc, char *argv[])
{
    printf("~~~ PRUEBAS ALUMNO ~~~\n");
    //prueba_abb_vacio();
    prueba_abb_guardar_borrar();
    return failure_count() > 0;
}
