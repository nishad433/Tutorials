#include <stdlib.h>
#include <stdio.h>
#include"linked_list.h"


node_t *list_add(node_t **head, int data){
    node_t *temp=(node_t *)malloc(sizeof(node_t));

    if(!temp)
        return NULL;

    temp->data=data;

    if(*head==NULL)
            *head=temp;
    else{
        node_t *iter=*head;
        while(iter->next!=NULL)
                iter=iter->next;
        iter->next=temp;
    }
    return temp;
}
