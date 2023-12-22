#include "stack.h"





int stack_init(Stack *stack, Pic *pic)
{
    stack->pic = pic;
    stack->next = NULL;
    return 0;
}


Stack *stack_create(Pic *pic)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL)
        return NULL;
    stack_init(stack, pic);
    return stack;
}
int stack_clear(Stack *stack)
{
    if (stack == NULL)
    {
        return -1;
    }
    while (stack->next != NULL)
    {
        stack_pop(stack);
    }
    free(stack);
    stack = NULL;
    return 0;
}

int stack_push(Stack *stack, Stack *node)
{
    if (stack == NULL || node == NULL)
    {
        return -1;
    }
    if (stack->next == NULL)
    {
        stack->next = node;
        return 0;
    }
    else
    {
        node->next = stack->next;
        stack->next = node;

        return 0;
    }
    return -1;
}
Stack *stack_pop(Stack *stack)
{
    if (stack == NULL)
    {
        return NULL;
    }
    if (stack->next == NULL)
    {
        return NULL;
    }
    else
    {
        Stack *node = stack->next;
        stack->next = node->next;
        node->next = NULL;
        return node;
    }
}


Stack *stack_top(Stack *stack)
{
    if (stack == NULL)
    {
        return NULL;
    }
    if (stack->next == NULL)
    {

        return NULL;
    }
    else
    {

        return stack->next;
    }
}




int stack_is_empty(Stack *stack)
{
    return stack->next == NULL;
}



// 刷新触摸屏的缓存区
void refresh_touch_screen_cache(int fd)
{
}


