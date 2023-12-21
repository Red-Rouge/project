#ifndef __LCD_H
#define __LCD_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define WHITE 0xFFffffff //白
#define BLACK 0xFF000000 //黑
#define RED 0xffff0000 //红
#define GREEN 0xff00ff00 //绿
#define BLUE 0xff0000ff //蓝
#define YELLOW 0xffffff00 //黄
#define CYAN 0xff00ffff //青
#define MAGENTA 0xffff00ff //品红
#define  TRANSPARENT 0x00000000 //透明

struct lcd
{
    unsigned int width; //宽
    unsigned int height;  //长
    int lcd_fb; //
    char *mmap_addr; //内存映射
    struct fb_fix_screeninfo *fixinfo; // 固定属性
    struct fb_var_screeninfo *varinfo; // 可变属性
};
typedef struct lcd Lcd;


// 打开LCD
Lcd *lcd_open();
//关闭lcd
int lcd_close(Lcd *lcd);
//lcd信息输出
void lcd_info(Lcd *lcd);
//lcd清屏
void lcd_clear(Lcd *lcd, unsigned int color);
// 画点
void lcd_draw_point(Lcd *lcd, int x, int y, unsigned int color);
// 画线
void lcd_draw_line(Lcd *lcd, int x1, int y1, int x2, int y2, unsigned int color);
// 画圆
void lcd_draw_circle(Lcd *lcd, int centerX, int centerY, int radius, unsigned int color);
// 获取指定点得颜色
int lcd_get_point(Lcd *lcd, int x, int y);

#endif
