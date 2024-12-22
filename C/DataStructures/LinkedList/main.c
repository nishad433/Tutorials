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
    int data,ret;
    printf("Enter Data to delete:");
    scanf("%d",&data);
    ret=list_del(&head, data);
    if(ret){
        disp_List();
    }
}

void disp_List()
{
    node_t *tmp = head;
    while(tmp){
        printf("(%p,%d)%s",tmp,tmp->data,(tmp->next)?"->":"\n");
        tmp=tmp->next;
    }
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
        printf("5. Exit\n");
        printf("Choice[1-5]:");
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
            case 5:
            default:
                    break;
        }
    }while(ch != 5);

    return 0;
}
