#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct node {
    int data;
    struct node *next;
}node_t;

node_t *list_add(node_t **head, int data);

#endif
