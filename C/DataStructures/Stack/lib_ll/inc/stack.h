#ifndef _STACK_H_
#define _STACK_H_

int StackInit(int count);
int isStackFull();
int isStackEmpty();
int getTopIndex();
void push(int val);
int pop();
int peak(int index);
void stackParams();
#endif //_STACK_H_
