#include<stdio.h>

#include <assert.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[]){


    int fd,ret;
    struct stat statbuf;

    assert(argc >= 2);

    const char *filepath=argv[1];


    // Open the File
    fd = open(filepath, O_RDWR);

    assert(fd >= 0);

    // Get the File Stats
    ret = fstat(fd, &statbuf);

    assert(ret == 0);

    printf("File=%s Size=%ld Bytes\n",filepath,statbuf.st_size);

    char *ptr = mmap(NULL, statbuf.st_size, PROT_WRITE, MAP_SHARED,
                  fd, 0);

    assert(ptr != MAP_FAILED);

    // Reversing the Content of File
    for(int i = 0; i < statbuf.st_size/2; i++){
	int j =	statbuf.st_size -i - 1;
	char ch = ptr[i];
	ptr[i]=ptr[j];
	ptr[j]=ch;
    }

    //Writing the File content to stdout(fd=1)
    ret = write(1, ptr, statbuf.st_size);

    assert(ret == statbuf.st_size);

    ret = munmap(ptr, statbuf.st_size);

    assert(ret == 0);
    
    close(fd);

    return 0;
}
