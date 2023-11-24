#include <stdio.h>
#include <stack.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int count;
	int top;
	int *ptr;
}stack_t;

stack_t *S = NULL;

int StackInit(int count){
	int ret = -1;
	if(count && S==NULL){
		S = malloc(sizeof(stack_t));
		memset(S, 0x0, sizeof(stack_t));
		S->ptr = (int *)malloc(count*sizeof(int));
		if(S->ptr){
			S->count = count;
			S->top = -1;
			ret = 0;
		}else{
			free(S);
			S = NULL;
		}
	}
	return ret;
}

int isStackInitialized(){
	if(S && S->ptr && S->count){
		return 1;
	}
	return 0;
}

int isStackFull(){
	if(isStackInitialized() && (S->top + 1) < S->count){
		return 0;
	}
	return 1;
}

int isStackEmpty(){
	if(isStackInitialized() && (S->top < 0 )){
		return 1;
	}
	return 0;
}

void push(int val){
	if(isStackInitialized()){
		if(!isStackFull()){
			S->ptr[++(S->top)] = val;
		}
	}
}

int pop(){
	int val = 0;
	if(isStackInitialized() && !isStackEmpty()){
		val = S->ptr[S->top];
		S->ptr[S->top] = 0;
		--(S->top);
	}
	return val;
}

int peak(int index){
	int ret = 0;
	if(isStackInitialized() && !isStackEmpty()){
		if(index <= S->top){
			return S->ptr[index];
		}
	}
	return ret;
}

int getTopIndex(){
	if(isStackInitialized()){
		return S->top;
	}
	return -2;
}
void stackParams(){
	if(S){
		printf("S=%p, count=%d, top=%d, S->ptr=%p\n",S, S->count, S->top, S->ptr);
	}
}
