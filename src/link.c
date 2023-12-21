#include "link.h"








int link_init(Link *node, Pic *pic)
{
    if (node == NULL || pic == NULL)
    {
        node->next = node;
        node->prev = node;
        return -1;
    }

    node->pic = pic;
    node->next = node;
    node->prev = node;
    return 0;
}
Link *link_create(Pic *pic)
{
    Link *node = (Link *)calloc(1, sizeof(Link));

    link_init(node, pic);
    return node;
}


int link_free(Link *head)
{
    Link *node = head->next;

    while (node != head)
    {
        head->next = node->next;
        if (node->pic != NULL)
        {
            pic_close(node->pic);
        }
        free(node);
        node = head->next;
    }
    free(head);
    return 0;
}

int link_printf(Link *head);

//头插
int link_add_head(Link *head, Link *node)
{
    if (head == NULL || node == NULL)
        return -1;

    if (head->next == NULL)
    {

        //只有一个节点

        head->next = node;
        node->prev = head;
        node->next = head;
        head->prev = node;
        return 0;
    }
    else
    {
        //多个节点
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
        return -1;
    }
    return 0;
}
//尾插
int link_add_tail(Link *head, Link *node)
{
    if (head == NULL || node == NULL)
        return -1;
    if (head->next == NULL)
    {
        //只有一个节点
        head->next = node;
        node->prev = head;
        node->next = head;
        head->prev = node;
        return 0;
    }
    else
    {
        //多个节点
        node->next = head;
        node->prev = head->prev;
        head->prev->next = node;
        head->prev = node;
        return 0;
    }
}