#ifndef __PIC_H
#define __PIC_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "lcd.h"
struct pic
{
    char *path; //图片路径
    int pic_fd; //图片文件标识符
    int *lcd_buf; //图片显示数组
    int *lcd_buf_back; //图片底色
    char *bmp_buf;
    int x_start;
    int y_start;
    float display_degree; //显示度
    int32_t size;// 图像文件大小
    int32_t offbits; // bmp图像数据偏移量
    int32_t width;  // 图像宽
    int32_t height; // 图像高
    int16_t bit_count; // 色深
    int32_t size_img; // bmp数据大小，必须是4的整数倍
};
typedef struct pic Pic;



// 生成新的图片
Pic *pic_new(char *path);
//关闭图片
int pic_close(Pic *pic);
// 输出图片信息
int pic_info(Pic *pic);

// 用底色填充清理
int pic_clear(Pic *pic, Lcd *lcd);
// 在指定位置显示图片
Pic *pic_show(Pic *pic, Lcd *lcd, int x, int y);
//圆形区域清除
void pic_draw_circle(Lcd *lcd, int centerX, int centerY, int radius, Pic *pic);






#endif


