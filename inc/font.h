#ifndef __font_h__
#define __font_h__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include "lcd.h"
#include "pic.h"
#define Color u32
#define getColor(a, c, b, d) (a|b<<8|c<<16|d<<24)
//  透明，蓝，绿，红

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef struct stbtt_fontinfo
{
    void *userdata;
    unsigned char *data;              // pointer to .ttf file
    int fontstart;         // offset of start of font

    int numGlyphs;                     // number of glyphs, needed for range checking

    int loca, head, glyf, hhea, hmtx, kern; // table locations as offset from start of .ttf
    int index_map;                     // a cmap mapping for our chosen character encoding
    int indexToLocFormat;              // format needed to map from glyph index to glyph
} stbtt_fontinfo;


typedef struct
{
    u32 height;
    u32 width;
    u32 byteperpixel;
    u8 *map;
    u32 color;
}bitmap;
typedef bitmap Canvas;//画布

typedef struct
{
    stbtt_fontinfo *info;
    u8 *buffer;
    float scale;
}font;
typedef font Font;

//lcd设备结构体
struct LcdDevice
{
    int fd;
    unsigned int *mp; //保存映射首地址

};

//初始化Lcd
struct LcdDevice *init_lcd(const char *device);
//1.初始化字库 
font *fontLoad(char *fontPath);

//2.设置字体的大小 
void fontSetSize(font *f, s32 pixels);

//3.设置字体输出框的大小
bitmap *createBitmap(u32 width, u32 height, u32 byteperpixel);

//可以指定输出框的颜色
bitmap *createBitmapWithInit(u32 width, u32 height, u32 byteperpixel, Color c);


//4.把字体输出到输出框中
void fontPrint(font *f, bitmap *screen, s32 x, s32 y, char *text, Color c, s32 maxWidth);

//5.把输出框的所有信息显示到LCD屏幕中 
void show_font_to_lcd(unsigned int *p, int px, int py, bitmap *bm);

// 关闭字体库
void fontUnload(font *f);

// 关闭bitmap
void destroyBitmap(bitmap *bm);



//打开字体库，path是路径
Font *font_open(char *path);
//设置字体的大小
int font_size(Font *f, int pixels);
/*把字体格式化输出到输出框中，font：字体，canvas:输出框，x，y：坐标，Color颜色，
maxWidth：一行最多显示的字体数量，默认为0，是全字符显示，test：显示的文本，支持格式化*/
int font_printf(Font *font, Canvas *canvas, int x, int y, unsigned int Color, int maxWidth, char *text, ...);
int font_printf_lcd(Font *font, Lcd *lcd, int x, int y, unsigned int Color, int maxWidth, char *text, ...);
// 关闭字体库
void font_close(Font *font);

// 创建画布，width，height：长宽，bytesperpixel：像素，Color颜色
Canvas *canvas_create(int width, int height, int bytesperpixel, unsigned int Color);
// 关闭画布
void canvas_close(Canvas *canvas);

// 将画布输出到设备,x，y是坐标
Pic *canva_show(Canvas *canvas, Lcd *lcd, int x, int y);

#endif