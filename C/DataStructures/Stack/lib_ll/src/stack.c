#include <stdio.h>
#include <stack.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	int val;
	struct node *next;	
}node_t;

typedef struct {
	int count;
	node_t *head;
}stack_t;

stack_t *S = NULL;

int StackInit(int count){
	S = (stack_t *)malloc(sizeof(stack_t));
	if(S){
		memset(S, 0x0, sizeof(stack_t));
		S->head = NULL;
	}
	return 0;
}

int isStackInitialized(){
	if(S){
		return 1;
	}else{
		return 0;
	}
}

int isStackFull(){
	return 0;
}

int isStackEmpty(){
	if(isStackInitialized() && S->head ==NULL){
		return 1;
	}else{
		return 0;
	}
}

void insertNode(int val){
	node_t *head = S->head;
	node_t *tmp=NULL;
	tmp = malloc(sizeof(node_t *));
	if(tmp){
		tmp->val = val;
		if(head){
			tmp->next = head;
		}else{
			tmp->next = NULL;
		}
		S->count++;
		S->head = tmp;
	}
}

void push(int val){
	if(isStackInitialized()){
		insertNode(val);
	}
}

int pop(){
	int val=-1;
	node_t *head = S->head;
	if(head){
		val = head->val;
		S->head = head->next;
		free(head);
		S->count--;
	}
	return val;
}

int peak(int index){
	int val=-1;
	node_t *head = S->head;
	while(head && index !=0){
		--index;
		head = head->next;
	}
	if(index==0 && head){
		val = head->val;
	}
	return val;
}

int getTopIndex(){
	return 0;
}

void stackParams(){
	printf("Count=%d, head=%p\n",S->count, S->head);
}
