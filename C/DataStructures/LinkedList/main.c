#include<stdio.h>
#include "linked_list.h"

node_t *head=NULL;


void add_Item()
{
    int data;
    node_t *node;
    printf("Enter Data:");
    scanf("%d",&data);
    node=list_add(&head, data);
    printf("Node added @ %p\n",node);
}

void delete_Item()
{

}

void disp_List()
{
    do{
        printf("data @%p is %d\n",head,head->data);
        head=head->next;
    }while(head!=NULL);
}

void search_Item()
{

}


int main()
{
    int ch;
    do
    {
        printf("Linked List\n");
        printf("1. Add Item\n");
        printf("2. Delete Item\n");
        printf("3. Display List\n");
        printf("4. Search Item\n");
        printf("Choice[1-4]:");
        scanf("%d",&ch);
        switch(ch){
            case 1:
                    add_Item();
                    break;
            case 2:
                    delete_Item();
                    break;
            case 3:
                    disp_List();
                    break;
            case 4:
                    search_Item();
                    break;
        }
        printf("Continue[0/1]:");
        scanf("%d",&ch);
    }while(ch == 1);

    return 0;
}
