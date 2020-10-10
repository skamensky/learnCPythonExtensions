#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
    char *last_4 = calloc(4+1,sizeof(last_4));
    strcpy(last_4,"****");
    last_4[4]='\0';

//    printf("%zu",strlen(last_4));
    strlen("****");
    char i[] = "****";
    printf("%c\n",i[4]);
    return 0;
}