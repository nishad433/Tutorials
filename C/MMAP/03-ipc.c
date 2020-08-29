#include<stdio.h>

#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 10

int main(int argc, char *argv[]){

    int *ptr = mmap(NULL, N*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
                  0, 0);

    assert(ptr != MAP_FAILED);

    pid_t pid=fork();
    assert(pid >= 0);

    if(pid == 0){
	// Child Process
	printf("Child Process\t");
	for(int i=0; i< N;i++){
	    ptr[i]=i;
	    printf("%d",ptr[i]);
	}

    }else {
	// Parent Process

	waitpid(pid,NULL,0);	
	printf("\nParent Process\t");
	for(int i=0; i< N;i++){
	    printf("%d",ptr[i]);
	}	
	int retval = munmap(ptr, N*sizeof(int));
	assert(retval == 0);


    }
    
        return 0;
}
