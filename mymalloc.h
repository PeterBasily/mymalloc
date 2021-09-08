/*
STUDENT: Peter Basily
RUID: 169006568
*/
#define malloc(x) mymalloc(x,__FILE__,__LINE__)
#define free(x) myfree(x,__FILE__,__LINE__) 
#define MEM_SIZE 4096

void *mymalloc(size_t size,char *file, int line);
void myfree(void *pointer, char *file, int line);