#include<stdio.h>
#include<stack.h>
#include<stdlib.h>

int main(){
	if(StackInit(10)<0){
		printf("Stack initialization failed\n");
		exit(-1);
	}

	stackParams();
	push(1);
	stackParams();
	push(2);
	stackParams();
	printf("Peaked value @ %d=%d\n",0,peak(0));
	printf("Peaked value @ %d=%d\n",1,peak(1));
	printf("Peaked value @ %d=%d\n",2,peak(2));
	printf("Popped value=%d\n",pop());
	stackParams();
	printf("Popped value=%d\n",pop());
	stackParams();
	printf("Popped value=%d\n",pop());
	stackParams();
	return 0;
}
