#include<stdio.h>

#include <assert.h>
#include <sys/mman.h>


#define N 10

int main(int argc, char *argv[]){

    int *ptr=mmap(NULL, N*sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
                  0, 0);

    assert(ptr != MAP_FAILED);

    assert(N >=2);
    ptr[0]=0;
    ptr[1]=1;

    for(int i = 2; i < N; i++){
	    ptr[i]=ptr[i-2] + ptr[i-1];
    }
    
    int retval = munmap(ptr, N*sizeof(int));

    assert(retval == 0);

    return 0;
}
