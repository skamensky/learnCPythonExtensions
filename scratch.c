#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int main(){
    if(test_prime_generator()){
        printf("The first 1k prime numbers match\n");
    }
    else{
        printf("WARNING: Prime numbers did not match\n");
    }
    return 0;
}