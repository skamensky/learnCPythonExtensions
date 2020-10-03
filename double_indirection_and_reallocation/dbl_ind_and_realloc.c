#include <stdio.h>
#include <stdlib.h>

typedef  struct key_val{
    int k;
    int v;
} key_val;

static key_val **map;


void do_for_static_structs(void){

    int numOfPointers;

    printf("Enter the number of pointers to allocate: ");
    scanf("%d", &numOfPointers);

    map = malloc((sizeof map) * numOfPointers);
    for(int i=0;i<numOfPointers;++i){
        key_val *kv = malloc(sizeof(struct key_val));
        kv->k=i;
        kv->v=i*i;
        map[i]=kv;
    }

    int newNumOfPointers;
    printf("Enter the new number of pointers to allocate to the same memory address: ");
    scanf("%d", &newNumOfPointers);
    int totalNumOfPointers = numOfPointers+newNumOfPointers;
    map = realloc(map,(sizeof map)*totalNumOfPointers);


    for(int i=numOfPointers;i<totalNumOfPointers;++i){
        key_val *kv = malloc(sizeof(struct key_val));
        kv->k=i;
        kv->v=i*i;
        map[i]=kv;
    }

    for(int i =0;i<totalNumOfPointers;i++){
        key_val *kv = *(map+i);
        printf("Val of pointer at dptr[%d] is {k=%d;v=%d}\n",i,kv->k,kv->v);
        free(map[i]);
    }

    free(map);
}

void do_for_doubles(void){
    int numOfPointers;

    printf("Enter the number of pointers to allocate: ");
    scanf("%d", &numOfPointers);

    int **dptr = malloc((sizeof dptr) * numOfPointers);
    for(int i=0;i<numOfPointers;++i){

        int *num = malloc(sizeof num);
        *num = (i+1)*10;
        dptr[i]=num;
    }

    int newNumOfPointers;
    printf("Enter the new number of pointers to allocate to the same memory address: ");
    scanf("%d", &newNumOfPointers);
    int totalNumOfPointers = numOfPointers+newNumOfPointers;
    dptr = realloc(dptr,(sizeof dptr)*totalNumOfPointers);


    for(int i=numOfPointers;i<totalNumOfPointers;++i){
        int *num = malloc(sizeof num);
        *num = (i+1)*10;
        dptr[i]=num;
    }

    for(int i =0;i<totalNumOfPointers;i++){
        printf("Val of pointer at dptr[%d] is %d\n",i,**(dptr+i));
        free(dptr[i]);
    }
    free(dptr);
}

int main(){
    do_for_doubles();
    do_for_static_structs();
    return 0;
}


