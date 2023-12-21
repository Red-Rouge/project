#ifndef __2048_H
#define __2048_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pic.h"
#include "lcd.h"
#include "stack.h"
#include "touch.h"
#include "font.h"






struct frame
{
    int num;
    struct coor *start, *end;
};
typedef struct frame Frame;

void game(Lcd *lcd, Stack *stack, Touch *touch, int num);


#endif


