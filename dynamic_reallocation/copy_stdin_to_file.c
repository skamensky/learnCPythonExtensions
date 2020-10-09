#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct input_container {
    char *text;
    char *target_path;
} input_container;

input_container *init_input_container(char *text, char *target_path){
    input_container *ic = malloc(sizeof(ic));
    if(ic==NULL){
        return NULL;
    }
    ic->target_path=target_path;
    ic->text=text;
    return ic;
}

void destroy_input_container(input_container *ic){
    free(ic->text);
    free(ic);
}

void write_text_to_file(input_container *ic){
    FILE *fp = fopen(ic->target_path,"w");
    fputs(ic->text,fp);
}

void log_text(char* prefix,char *text){
    FILE *fp = fopen("./log.txt","a");
    fprintf(fp,"%s%s\n",prefix,text);
//    fflush(fp);
    fclose(fp);
}

void shift_chars_to_left(char *source,char new_end_char){
    for(int i=0;i<strlen(source)-1;++i){
        source[i]=source[i+1];
    }
    source[strlen(source)-1]=new_end_char;
}

char *read_from_input(void){
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("Type your message (it can be as large as you want!). Type DONE and hit enter when you're done\n");
    printf("------------------------------------------\n");
    size_t buffer = 8;
    char *input = malloc(sizeof(input)*buffer);
    if(input==NULL){
        printf("hi\n");
        return NULL;
    }
    int current_char;
    size_t current_size = 0;

    char *end_string = "DONE";
    char *last_4 = calloc(4+1,sizeof(last_4));
    strcpy(last_4,"****");
    last_4[4]='\0';

    while(strcmp(last_4,end_string)){
        current_char=fgetc(stdin);

        char *char_log = malloc(2);
        char_log[0]=(char)current_char;
        char_log[1]='\0';
        log_text("Current char is: ",char_log);
        free(char_log);

        shift_chars_to_left(last_4,current_char);
        log_text("Last 4 chars: ",last_4);

        input[current_size]=current_char;
        current_size++;
        //next iteration would result in overflow, reallocate memory
        if(current_size==buffer){
            buffer+=8;
            input = realloc(input,buffer);
            if(input==NULL){
                return NULL;
            }
        }
    }
    printf("\n");

    free(last_4);
    last_4=NULL;
    //remove the 'DONE' with room for a string terminator
    input = realloc(input,current_size-3);
    fflush(stdout);

    if(input==NULL){
        return NULL;
    }
    input[current_size-4]='\0';
    return input;
}

int main(int argc,char *argv[]){
    if(argc!=2){
        printf("Provide one and only one argument (the target path)\n");
        return 1;
    }

    char *text_input = read_from_input();
    if(text_input==NULL){
        printf("Out of memory\n");
        return 1;
    }
    input_container *ic = init_input_container(text_input,argv[1]);
    if(ic==NULL){
        printf("Out of memory\n");
        return 1;
    }
    write_text_to_file(ic);
    destroy_input_container(ic);
    return 0;
}