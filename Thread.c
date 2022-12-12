#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#define MAX_THREAD_NUM 20
struct Sequence{
    int len;
    char c;
};
struct FileCount{
    char * filename;
    int count;
};
void* print_char(void * thread_arg){
    struct Sequence * data;
    data = (struct Sequence*)thread_arg;
    for(int i = 0;i<data->len;i++){
        putc((int)data->c,stdout);
    }
    return NULL;
}
struct Sequence *Get_seq(){
    struct Sequence * seq;
    seq = (struct Sequence*)malloc(sizeof(struct Sequence));
    printf("\nType char for sequence: ");
    scanf("  %c",&seq->c);
    printf("\nType lenght of sequence: ");
    scanf(" %d",&seq->len);
    return seq;
}

void * count_char(void * thread_arg){
    thread_arg = (struct FileCount*)thread_arg;    
    struct FileCount * data = thread_arg;
    FILE * ifile = fopen(data->filename,"r");
    char * nname = data->filename;
    char ext[5] = "numc";
    do{
        data->count += 1;
    }while(fgetc(ifile) != EOF);
    fclose(ifile);
    for(int i = strlen(nname)-1;i>0;i--){
        if(nname[i]!="."){
            nname[i] = NULL;
        }else{break;}
    }
    strcat(nname,ext);
    FILE * ofile = fopen(nname,"w");
    fprintf(ofile,"%d",data->count);
    fclose(ofile);
    return data;
}

int main(){
    pthread_t thread_id;
    pthread_t threads[MAX_THREAD_NUM];
    struct FileCount *file,*a;
    char choise;
    int i;
    signal(SIGINT,&main);
    printf("\ns)Sequence\nf)File Count\nq)Quit\nChoose an option:");
    scanf(" %c",&choise);
    switch(choise){
        case 's':
            printf("\nHow many sequence do you want to print?: ");
            scanf("%d",&i);
            if(i > 20){printf("\nTo many thread to create, please type a number equal or smaller than 20.\n");break;}
            struct Sequence **seq;
            seq = (struct Seqeuence*)malloc(sizeof(struct Sequence*));
            for(int j = 0;j<i;j++){
                seq[j] = Get_seq();
            }
            do{
                for(int j = 0;j<i;j++){pthread_create(&threads[j],NULL,&print_char,(void*)seq[j]);}           
                for(int j = 0;j<i;j++){pthread_join(threads[j],NULL);}
            }while(1);
            break;
        case 'f':
            file = (struct FileCount*)malloc(sizeof(struct FileCount));
            file->filename = (char*)malloc(20);
            printf("\nType the name of the file(include extension): ");
            fscanf(stdin,"%s",file->filename);
            printf("\n");
            pthread_create(&thread_id,NULL,&count_char,(void*)file);
            pthread_join(thread_id,&a);
            printf("%d",a->count);
            free(file);
        break;
        case 'q':
            printf("\nGoodbye\n");
            exit(0);
        default:
            printf("\nInvalid character typed, try again.\n");
        }
}