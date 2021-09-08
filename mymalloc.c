
#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include "mymalloc.h"


typedef struct block
{   
    struct block *next; //the next block
    unsigned short size; //the size of the memory block
    unsigned short isfree; // 1 if the memory is free 0 if it is not  
}memblock; //this is metadata for our memory



static char mem[MEM_SIZE]; //our total memory pool

 memblock *root = NULL; //our entry point to the memory

void init() //initializes the memory
{
    root = (void*)mem; //sets the address of root to the start of our memory
    root->size = MEM_SIZE - sizeof(memblock); //the size of our adressable memory is 4096 - the size of a memory block
    root->isfree = 1; //makes the memory available
    root->next = NULL; //our next memory node is null until we need to address it
}
//function used for testing. Prints the current state of the memory.
void printmem(memblock* block)
{
    if(block == NULL)
    {
        return; 
    }
    while(block->next != NULL)
    {
        printf("|A: %p , free: %d, size: %d|\n", block, block->isfree, block->size);
        printf("                 |\n");
        block = block->next;
    }
    printf("|A: %p , free: %d, size: %d|\n", block, block->isfree, block->size);
    
}


/*=========================start of mymalloc==============================*/
void *mymalloc(size_t bytes, char *file, int line) //bytes: the number of bytes to be allocated, file: place holder for __FILE__, line: place holder for __LINE__ (defined in the header file)
{
    
    if(root == NULL) //if mymalloc isn't previously called, our root node will be null
    {
        init(); //run our initialization function
        if(bytes <= 0) //if the bytes to be allocated are <= 0, return NULL
        {
            return NULL;
        }
        
        if(root->size >= (bytes + sizeof(memblock)+1)) //if we have enough space for another memblock and 1 byte, create the next block
        {
            memblock *c = ((void*)(root)) + sizeof(memblock) + bytes;
            c->size = root->size - sizeof(memblock) - bytes;
            c->isfree = 1;
            c->next = NULL;
            root->isfree = 0;
            root->size = (unsigned short)bytes;
            root->next = c;
            void * r = ((void*)root);
           // printf("memory allocated for %s at line %d of size %d\n",file,line, root->size); 
            return ++r;
        }
        else if(root->size >= bytes) //otherwise, allocate the remaining memory to the request
        {
            root->isfree = 0;
            root->next = NULL;
            void *r = ((void*)root);
           // printf("memory allocated for %s at line %d of size %d\n",file,line, root->size);
            return ++r;
        }
        
        else
        {
            printf("unable to allocate memory in file %s at line %d\n", file, line); //otherwise, the memory requested was too big, so print out an error message
            
        }
        
       
        
    }
    else //otherwise, the memory is already initialized
    {  
        if(bytes <= 0) //if the bytes requested are <= 0, return NULL
        {
            return NULL;
        }
      
        memblock *temp = root; // used to iterate through our memory so that we don't change our root pointer
        while((temp->size)) //so long as temp has a size (meaning it is initiated)
        {
          
            if(temp->isfree == 1) //if the memory is free
            {  
               
                if((temp->size >= bytes + sizeof(memblock)+1) && temp->next == NULL) //if the next block is NULL, and we have enough space for our byte request and another memory block, create a new block
                {
                    memblock *next = ((void*)(temp)) + sizeof(memblock) + bytes;
                    temp->isfree = 0;
                    next->isfree = 1;
                    next->size = temp->size - (bytes + sizeof(memblock));
                    temp->size = (unsigned short)bytes;
                   // printf("memory allocated for %s at line %d of size %d \n",file,line, temp->size);
                    temp->next = next;
                    memblock *r = ((void*)temp);
                    return ++r;                             

                }
                else if(temp->size >= bytes+sizeof(memblock)+1) //otherwise, if our next block is not NULL, but we can break the current block into smaller blocks, create a new block and insert it
                {
                  
                        memblock *c = ((void*)(temp)) + sizeof(memblock) + bytes;
                        c->size = temp->size - (sizeof(memblock) + bytes);
                        c->isfree = 1;
                        c->next = temp->next; //this makes our new block exist before temp->next
                        temp->size = (unsigned short)bytes;
                        temp->isfree = 0;
                        temp->next = c; //and this points our temp->next to our newly created block
                        memblock *r = ((void*)temp);
                      //  printf("memory allocated for %s at line %d of size %d\n",file,line, temp->size);
                        return  ++r;
                }
                else if(temp->size >= bytes) //otherwise if we don't have enough room for a new block, just return the address of our current free block
                {
                    temp->isfree = 0;
                  //  printf("memory allocated for %s at line %d of size %d\n", file, line, temp->size);
                    memblock *r = ((void*)temp);
                    return ++r; 
                }
                    
                     
            
                else //otherwise we checked our memory and it's too small
                {
                    if(temp->next == NULL) //if our next node is NULL, that means we reached the end of our memory and couldn't find a block so we print an error message
                    {
                        printf("unable to allocate memory in file %s, at line %d\n", file, line);
                        return NULL; //this is to break out of our while loop
                    }
                    else //if temp->next exists, change temp to temp->next, then the while loop will still do the checks on the node
                    { 
                        
                        temp = temp->next;
                    }
                } 
                    
            }
                
            
            else if((temp->isfree == 0 ||temp->size < bytes) && temp->next != NULL) //otherwise, if the memory is not available, or it's too small, and temp->next exists, go to temp->next
            {
                  
                temp = temp->next;
            }
           
            else if(temp->next == NULL) //print an error message if we reached the end of our memory
            {
                printf("unable to allocate memory in file %s at line %d\n",file, line);
                return NULL;
            }
            
        }
    } 
    
}

/*===============================================================start of myfree()=====================================================================*/

void myfree(void *pointer, char *file, int line) // *pointer: the pointer we want to free, file: placeholder for __FILE__, line: placeholder for __LINE__ (these are defined in the header file)
{
    if((void*)mem <= pointer && pointer <= (void*)mem + MEM_SIZE) //so long as our address is between the start of our memory and the end of it
    {
        memblock *prev= root; //previous will take the value of our previous node
        memblock *c = pointer; //we will save our void address to a memblock
        --c; //we'll then decrement the address
        memblock *temp = root; //we use temp to increment through our memory nodes without damaging our memory
        while(temp != NULL)
        {
            if(temp == c) //if the address we're trying to free matches an address managed by our malloc
            {
                
                if(temp->isfree == 1) //if the address is already free, we can't free it
                {
                
                    printf("Attempt to free memory in file %s at line %d failed, memory is already free\n", file, line);
                    return;
                }
                else 
                {
                    if(prev->isfree == 1) //if the address is not free, and the previous address is free, merge the current address with the previous address
                    {
                        prev->size = prev->size+sizeof(memblock)+temp->size;
                        prev->next = temp->next;
                        //printf("memory freed in file %s at line %d and merged with previous block\n", file, line);
                        return;
                    }
                    else //otherwise free the current address
                    {
                        //printf("memory freed in file %s at line %d\n", file, line); 
                        temp->isfree = 1;
                        return;
                    }
                    
                    
                }

                
            }
            else
            {
                prev = temp; //otherwise, set previous to our current address
                temp = temp->next;  //set our current address to our next address   
            }
            
        }


    }
    else if(pointer == NULL)
    {
        return;
    }
    else
    {
        printf("Attempt to free memory in file %s at line %d failed: memory not allocated by malloc\n", file, line);
        return;
    }
    
    
    
}



