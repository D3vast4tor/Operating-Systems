#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#define BUFFOK 0
#define EINIT 1
#define BUFFEMPTY 2
#define BUFFULL 3
#define INVCHAR 4
#define MAXTHREADS 50
typedef struct{
    int start;
    int end;
    int dim;
    int * data;
    pthread_mutex_t mutex;
}buffer_t;
//Demonstation only:
//declaring the buffer as a global variable
//being it a new type this could be implemented as list of buffers.
buffer_t * buffer;
//Initialize the given buffer of dimendion "dim".
//Return BUFFOK if succesfully initialized else return EINIT.
int buffer_init(int dim){
    buffer = (buffer_t*)malloc(sizeof(buffer_t));
    buffer->start = 0;
    buffer->end = 0;
    buffer->dim = dim;
    buffer->data = calloc(dim ,sizeof( unsigned int));
    pthread_mutex_init(&buffer->mutex,NULL);
    if(buffer != NULL){
        return BUFFOK;
    }else{
        return EINIT;
    }
}
//Put a value into the buffer avoiding the concurrency by putting
//eventual other action on the same buffer in mutual exclusion.
//Return BUFFOK on success or BUFFULL if buffer is full.
int bput(int value){
    //Entering critical section
    pthread_mutex_lock(&buffer->mutex);
    if(buffer->end <= buffer->dim){
        buffer->data[buffer->end] = value;
        printf("\n%d)Added value: %d | end:%d\n",buffer->end,buffer->data[buffer->end],buffer->end);
        buffer->end++; 
        //End of critical section
        pthread_mutex_unlock(&buffer->mutex);
        return BUFFOK;
    }else{
        //End of critical section
        pthread_mutex_unlock(&buffer->mutex);
        return BUFFULL;
    }
}
//Remove the first valid value of the buffer avoiding the concurrency 
//by putting eventual other action on the same buffer in mutual exclusion.
//Return BUFFOK on success or BUFFEMPTY if buffer is empty.
int bget(void *unused){
    //Entering critical section
    pthread_mutex_lock(&buffer->mutex);
    if(buffer->start <= buffer->dim){
        printf("\n%d | Removed value: %d\n",buffer->start,buffer->data[buffer->start]);
        buffer->data[buffer->start] = 0;
        buffer->start++;
        //End of critical section
        pthread_mutex_unlock(&buffer->mutex);
        return BUFFOK;
    }else{
        //End of critical section
        pthread_mutex_unlock(&buffer->mutex);
        return BUFFEMPTY;
    }
}

int main(){
    pthread_t threads[MAXTHREADS];
    int value;
    char * mult;
    mult =  malloc(10);
    int dim;
    signal(INVCHAR,&main);
    printf("\nValues must be inserted as positive integer followed by the notation Kb,Mb after the value.\nType the dimension of the buffer: ");
    scanf("%d %s",&value,mult);
    if(mult[0] == 'm' || mult[0] == 'M'){
        dim = value*(2048*2048);
    }else if(mult[0] == 'k' || mult[0] == 'K'){
        dim = value*2048;
    }else{
        printf("\nInvalid value typed...\nRemember to use the correct notation.\n");
        raise(INVCHAR);
    }
    buffer_init(dim);
    while(buffer->end <= buffer->dim){
        for(int j = 0;j<=MAXTHREADS;j++){
            pthread_create(&threads[j],NULL,&bput,rand());
            
        }
        for(int j = 0;j<=MAXTHREADS;j++){
            pthread_join(threads[j],NULL);
        }
        
    }
    while(buffer->start <= buffer->dim){
        for(int j = 0;j<=MAXTHREADS;j++){
            pthread_create(&threads[j],NULL,&bget,NULL);
        }
        for(int j = 0;j<=MAXTHREADS;j++){
            pthread_join(threads[j],NULL);
        }
    }
}