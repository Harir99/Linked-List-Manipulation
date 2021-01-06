#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct Performance *newPerformance(){
   struct Performance *perf; 
   perf = (struct Performance*) malloc(sizeof(struct Performance));
   // check if array is not empty, set reads, write, mallocd and frees to zero 
   if(perf != 0){
       perf->reads = 0;
       perf->writes = 0;
       perf->mallocs = 0;
       perf->frees = 0;
   }
   else {
       printf("Make newPreformance Error! Malloc Fail!\n");
       exit(0);
   }
    return perf;
}
void push( struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width ){
    int i = 0;
    struct Node *newNode;
    newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = malloc(width);
    newNode->next = NULL;
    if(newNode != 0 && newNode->data != 0){
        // copy width bytes of data from the parameter src to the variable data in the new Node
        for (i = 0; i < width; i++){
            ((char*)newNode->data)[i] = ((char*) src)[i];
        }
        newNode->next = *list_ptr;
        *list_ptr = newNode;
    }
    else {
        printf("push Error! Malloc Fail!\n");
        exit(0);
    }
    // increment writes
    performance->writes++;
    performance->mallocs++;

}
void readHead( struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width ){
    int i = 0;
    struct Node *newNode = *list_ptr;
  
    if(newNode != 0 ){
        // copy width bytes of data from the parameter src to the variable data in the new Node
        for (i = 0; i < width; i++){
            ((char*)dest)[i] = ((char*)newNode->data)[i];
        }
    }
    else {
        printf("readHead Error! empty list!\n");
        exit(0);
    }
    
    // increment reads
    performance->reads++;
}
void pop( struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width ){
    if(list_ptr == NULL){
        printf("Pop Error!! list is empty");
        exit(1);
    }
    struct Node *newNod = *list_ptr;
    readHead(performance,list_ptr,dest,width); 
    (*list_ptr) = (*list_ptr)->next;

    // increment frees
    performance->frees++;
    free(newNod->data);
    free(newNod);
}

struct Node **next( struct Performance *performance, struct Node **list_ptr){
    // check if list is empty then exit
    if((*list_ptr) == NULL){
        printf("Next Function Error!! empty list");
        exit(1);
    } else {
        // get pointer to the pointer to the second item in a list.
       list_ptr = &((*list_ptr)->next);
    }
    performance->reads++;
   return list_ptr;
}
int isEmpty( struct Performance *performance, struct Node **list_ptr ){

    return ((*list_ptr) != NULL) ? 0: 1;
}
void freeList( struct Performance *performance, struct Node **list_ptr ){
    int i = 0;
    for ( i = 0;(!isEmpty(performance,list_ptr)); i++)
    {
        pop(performance,list_ptr,0,0);
    }
}
void readItem( struct Performance *performance, struct Node **list_ptr,unsigned int index, void *dest, unsigned int width ){
    int i = 0;
    for ( i = 0; i < index; i++)
    {
        list_ptr = next(performance,list_ptr);
    }
    readHead(performance,list_ptr,dest,width);
}
void appendItem( struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width ){
    while(!isEmpty(performance,list_ptr)){
        list_ptr = next(performance,list_ptr);    
    }
        push(performance,list_ptr,src,width);
}

void insertItem( struct Performance *performance, struct Node **list_ptr, unsigned int index, void *src, unsigned int width ){
    int i = 0;
    for ( i = 0; i < index; i++)
    {
        list_ptr = next(performance,list_ptr);
    }
    push(performance,list_ptr,src,width);
}

void prependItem( struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width ){
    insertItem(performance,list_ptr,0,src,width);
}

void deleteItem( struct Performance *performance, struct Node **list_ptr, unsigned int index ){
    int i = 0;
    for ( i = 0; i < index; i++)
    {
        list_ptr = next(performance,list_ptr);
    }
    pop(performance,list_ptr,0,0);
}

int findItem( struct Performance *performance, struct Node **list_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width ){
   int i = 0;
   int result = 0;
   // allocate memory
   void* dest = (char*)malloc(width);
    while(!isEmpty(performance,list_ptr)){
        readHead(performance,list_ptr,dest,width);
        result = (*compar)(dest,target);
        if(result == 0){
            free(dest);
            return i;
        }
        list_ptr = next(performance,list_ptr);    
        i++;
    }
    free(dest);
    return -1;
}