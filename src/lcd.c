#include "lcd.h"






int lcd_init(Lcd *lcd)
{
    lcd->lcd_fb = open("/dev/fb0", O_RDWR);
    if (lcd->lcd_fb == -1)
    {
        perror("open");
        close(lcd->lcd_fb);
        return -1;
    }
    lcd->fixinfo = (struct fb_fix_screeninfo *)malloc(sizeof(struct fb_fix_screeninfo));
    lcd->varinfo = (struct fb_var_screeninfo *)malloc(sizeof(struct fb_var_screeninfo));
    if (ioctl(lcd->lcd_fb, FBIOGET_FSCREENINFO, lcd->fixinfo) != 0)
    {
        perror("ioctl");
        close(lcd->lcd_fb);
        return -1;
    }
    if (ioctl(lcd->lcd_fb, FBIOGET_VSCREENINFO, lcd->varinfo) != 0)
    {
        printf("test\n");

        perror("ioctl");
        close(lcd->lcd_fb);
        return -1;
    }
    lcd->height = lcd->varinfo->yres;
    lcd->width = lcd->varinfo->xres;
    lcd->mmap_addr = mmap(NULL, lcd->height * lcd->width * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd->lcd_fb, 0);
    return 0;
}

Lcd *lcd_open()
{
    Lcd *lcd = (Lcd *)calloc(1, sizeof(Lcd));
    if (lcd == NULL)
    {
        perror("calloc");
        return NULL;
    }
    else if (lcd_init(lcd) == -1)
    {
        free(lcd);
        return NULL;
    }
    return lcd;
}

int lcd_close(Lcd *lcd)
{
    if (lcd->lcd_fb != -1)
    {
        munmap(lcd->mmap_addr, lcd->height * lcd->width * 4);
        close(lcd->lcd_fb);
    }
    free(lcd->fixinfo);
    free(lcd->varinfo);
    free(lcd);
    return 0;
}



void lcd_clear(Lcd *lcd, unsigned int color)
{
    for (int i = 0; i < lcd->height * lcd->width; i++)
    {
        memcpy((void *)(lcd->mmap_addr + i * 4), &color, 4);
    }
}

void lcd_draw_point(Lcd *lcd, int x, int y, unsigned int color)
{
    if ((x > 0 && x < lcd->width) && (y > 0 && y < lcd->height))
        if (color == 0x00000000)
            memcpy((void *)(lcd->mmap_addr + y * lcd->width * 4 + x * 4), &color, 4);

}
void lcd_draw_line(Lcd *lcd, int x1, int y1, int x2, int y2, unsigned int color)
{
    for (int y = ((y1 < 0) ? 0 : y1);y < ((y2 > lcd->height) ? lcd->height : y2);y++)
    {
        for (int x = ((x1 < 0) ? 0 : x1);x < ((x2 > lcd->width) ? lcd->width : x2);x++)
        {
            lcd_draw_point(lcd, x, y, color);
        }
    }
}

void lcd_draw_circle(Lcd *lcd, int centerX, int centerY, int radius, unsigned int color)
{
    for (int y = ((centerY - radius) < 0 ? 0 : (centerY - radius));y <= ((centerY + radius) > lcd->height ? lcd->height - 1 : (centerY + radius));y++)
    {
        for (int x = ((centerX - radius) < 0 ? 0 : (centerX - radius));x <= ((centerX + radius) > lcd->width ? lcd->width - 1 : (centerX + radius));x++)
        {
            if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius)
            {
                lcd_draw_point(lcd, x, y, color);
            }
        }
    }
}



int lcd_get_point(Lcd *lcd, int x, int y)
{
    return *((unsigned int *)(lcd->mmap_addr + y * lcd->width * 4 + x * 4));
}























void lcd_info(Lcd *lcd)
{
    if (lcd->fixinfo == NULL || lcd->mmap_addr == NULL)
    {
        return;
    }
    printf("[ID]: %s\n", lcd->fixinfo->id);
    printf("[FB类型]: ");
    switch (lcd->fixinfo->type)
    {
    case FB_TYPE_PACKED_PIXELS:      printf("组合像素\n");break;
    case FB_TYPE_PLANES:             printf("非交错图层\n");break;
    case FB_TYPE_INTERLEAVED_PLANES: printf("交错图层\n");break;
    case FB_TYPE_TEXT:               printf("文本或属性\n");break;
    case FB_TYPE_VGA_PLANES:         printf("EGA/VGA图层\n");break;
    }
    printf("[FB视觉]: ");

    switch (lcd->fixinfo->visual)
    {
    case FB_VISUAL_MONO01:             printf("灰度. 1=黑;0=白\n");break;
    case FB_VISUAL_MONO10:             printf("灰度. 0=黑;1=白\n");break;
    case FB_VISUAL_TRUECOLOR:          printf("真彩色\n");break;
    case FB_VISUAL_PSEUDOCOLOR:        printf("伪彩色\n");break;
    case FB_VISUAL_DIRECTCOLOR:        printf("直接彩色\n");break;
    case FB_VISUAL_STATIC_PSEUDOCOLOR: printf("只读伪彩色\n");break;
    }
    printf("[行宽]: %d 字节\n", lcd->fixinfo->line_length);
    printf("\n获取LCD设备可变属性信息成功：\n");
    printf("[可见区分辨率]: %d×%d\n", lcd->varinfo->xres, lcd->varinfo->yres);
    printf("[虚拟区分辨率]: %d×%d\n", lcd->varinfo->xres_virtual, lcd->varinfo->yres_virtual);
    printf("[从虚拟区到可见区偏移量]: (%d,%d)\n", lcd->varinfo->xoffset, lcd->varinfo->yoffset);
    printf("[色深]: %d bits\n", lcd->varinfo->bits_per_pixel);
    printf("[像素内颜色结构]:\n");
    printf("[红] 偏移量:%d, 长度:%d bits\n", lcd->varinfo->red.offset, lcd->varinfo->red.length);
    printf("[绿] 偏移量:%d, 长度:%d bits\n", lcd->varinfo->green.offset, lcd->varinfo->green.length);
    printf("[蓝] 偏移量:%d, 长度:%d bits\n", lcd->varinfo->blue.offset, lcd->varinfo->blue.length);
    printf("[透明度] 偏移量:%d, 长度:%d bits\n", lcd->varinfo->transp.offset, lcd->varinfo->transp.length);
}





