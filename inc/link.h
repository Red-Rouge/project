#ifndef __LINK_H
#define __LINK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "pic.h"

struct link
{
    Pic *pic;
    struct link *next, *prev;
};
typedef struct link Link;


Link *link_create(Pic *pic);
int link_free(Link *head);

int link_printf(Link *head);

//头插
int link_add_head(Link *head, Link *node);
//尾插
int link_add_tail(Link *head, Link *node);



#endif


