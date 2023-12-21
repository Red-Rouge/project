#ifndef __STACK_H
#define __STACK_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "pic.h"

struct stack
{
    Pic *pic;
    struct stack *next;
};
typedef struct stack Stack;



Stack *stack_create(Pic *pic);
int stack_clear(Stack *stack);

int stack_push(Stack *stack, Stack *node);
Stack *stack_pop(Stack *stack);
Stack *stack_top(Stack *stack);
int stack_is_empty(Stack *stack);







#endif
