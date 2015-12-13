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
    pruebas_abb_alumno();

    return failure_count() > 0;
}
