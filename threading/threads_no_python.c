#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>



typedef struct {
    int sleep_seconds;
    pthread_t thread;
    int return_value;

} thread_with_data;

void *sleepy_square_with_struct(void *thread){
    int seconds = ((thread_with_data*)thread)->sleep_seconds;
    sleep(seconds);
    printf("Performing square in sleepy_square_with_struct after %d seconds.\n",seconds);
    ((thread_with_data*)thread)->return_value=seconds*seconds;
    return NULL;
}

void *sleepy_square_without_struct(void *seconds){
    int seconds_local = (*(int *)seconds);
    sleep(seconds_local);
    printf("Performing square in sleepy_square_without_struct after %d seconds.\n",seconds_local);
    int *square = malloc(sizeof square);
    *square = seconds_local*seconds_local;
    return (void *)square;
    return NULL;
}

void do_sleepy_squares_with_structs(void){
    int numThreads = 5;
    thread_with_data *threads[numThreads];


    for(int i=0;i<numThreads;++i){
        thread_with_data *t = malloc(sizeof *t);
        t->sleep_seconds = i+1;
        threads [i] = t;
        pthread_create(&threads[i]->thread,NULL,&sleepy_square_with_struct,(void *)t);
    }

    for(int i=0;i<numThreads;++i){
        pthread_join(threads[i]->thread,NULL);
        printf("Return from sleepy_square_with_struct with arg %d is %d\n",i+1,threads[i]->return_value);
        free(threads[i]);
        threads[i] = NULL;
    }
}

void do_sleepy_squares_with_no_structs(void){

    int num_threads = 5;
    pthread_t threads[num_threads];
    int *args[num_threads];

    for(int i=0;i<num_threads;++i){
        int *arg = malloc(sizeof *arg);
        *arg = i+1;
        args[i]=arg;
        pthread_create(&threads[i],NULL,&sleepy_square_without_struct,(void *)arg);
    }

    for(int i=0;i<num_threads;++i){

        void *result;

        pthread_join(threads[i],&result);

        printf("Return from sleepy_square_without_struct with arg %d is %d\n",*args[i],*(int *)result);
        free(result);
        free(args[i]);
    }
}

int main(){
    do_sleepy_squares_with_no_structs();
    do_sleepy_squares_with_structs();
}