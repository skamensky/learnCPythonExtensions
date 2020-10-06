#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fromStack(void *ptr) {
    int dummy;
    return ptr > &dummy;
}

typedef  struct test {
    char *text;
} test;

void mod(test *t){
    t->text="modiyfing the value of a char pointer in another stack frame";
}

int main(){
    test *t;
    t = malloc(sizeof(t));
    mod(t);
    printf("fromStack(t): %d\n",fromStack(t));
    printf("fromStack(t->text): %d\n",fromStack(t->text));
    //outputs:
    //fromStack(t): 0
    //fromStack(t->text): 0
    free(t);
    return 0;
}