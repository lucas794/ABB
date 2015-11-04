#include <stdlib.h>
#include <stdio.h>

void swap (void** x, void** y) {
	void* direccion_y = *y;
	*y = *x;
	*x = direccion_y;
}

int main(void){
    int a = 2;
    int b = 5;
    int* elementos[]= {&a,&b};
    swap((void**)&elementos[0],(void**)&elementos[1]);
    if(a==5&&b==2){
        printf("OK\n");
    }
    else{
        printf("NO OK\n");
    }
    printf("a=%i b=%i \n",*elementos[0],*elementos[1]);
    return 0;
}
