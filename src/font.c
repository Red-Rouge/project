#include "font.h"





Font *font_open(char *path)
{
    Font *font = fontLoad(path);

    if (font == NULL)
    {
        perror("Failed to load font");
        return NULL;
    }
    return font;
}

int font_size(Font *font, int pixels)
{
    fontSetSize(font, pixels);
    return pixels;
}


Canvas *canvas_create(int width, int height, int bytesperpixel, unsigned int color)
{
    unsigned char alpha = (color >> 24) & 0XFF;  // 取出透明位
    unsigned char red = (color >> 16) & 0XFF;  // 取出红
    unsigned char blue = (color >> 8) & 0XFF;  // 取出蓝
    unsigned char green = (color >> 0) & 0XFF;  // 取出绿色
    Canvas *canvas = createBitmapWithInit(width, height, bytesperpixel, getColor(alpha, blue, green, red));
    if (canvas == NULL)
    {
        perror("Failed to create canvas");
        return NULL;
    }
    canvas->color = color;
    return canvas;
}


int font_printf(Font *font, Canvas *canvas, int x, int y, unsigned int color, int maxWidth, char *text, ...)
{
    // 将可变参数列表与text组合起来
    unsigned char alpha = (color >> 24) & 0XFF;  // 取出透明位
    unsigned char red = (color >> 16) & 0XFF;  // 取出红
    unsigned char blue = (color >> 8) & 0XFF;  // 取出蓝
    unsigned char green = (color >> 0) & 0XFF;  // 取出绿色
    va_list args;
    va_start(args, text);
    char buffer[200];
    int ret = vsprintf(buffer, text, args);
    fontPrint(font, canvas, x, y, buffer, getColor(alpha, blue, green, red), maxWidth);
    va_end(args);
    return -1;
}
int font_printf_lcd(Font *font, Lcd *lcd, int x, int y, unsigned int color, int maxWidth, char *text, ...)
{
    // 将可变参数列表与text组合起来
     // 将可变参数列表与text组合起来
    unsigned char alpha = (color >> 24) & 0XFF;  // 取出透明位
    unsigned char red = (color >> 16) & 0XFF;  // 取出红
    unsigned char blue = (color >> 8) & 0XFF;  // 取出蓝
    unsigned char green = (color >> 0) & 0XFF;  // 取出绿色
    va_list args;
    va_start(args, text);
    char buffer[200];
    int ret = vsprintf(buffer, text, args);
    Canvas *canvas = canvas_create(800, 480, 4, WHITE);
    strcpy(canvas->map, lcd->mmap_addr);
    fontPrint(font, canvas, x, y, buffer, getColor(alpha, blue, green, red), maxWidth);
    va_end(args);
    canvas_close(canvas);
    return -1;
}


Pic *canva_show(Canvas *canvas, Lcd *lcd, int x, int y)
{
    Pic *pic = (Pic *)calloc(1, sizeof(Pic));
    pic->path = (char *)calloc(10, sizeof(char));
    strcpy(pic->path, "font");
    pic->pic_fd = 0;
    pic->x_start = x;
    pic->y_start = y;
    pic->size = canvas->width * canvas->height * 4;
    pic->offbits = 0;
    pic->width = canvas->width;
    pic->height = canvas->height;
    pic->bit_count = 32;
    pic->size_img = canvas->width * canvas->height * 4;
    pic->display_degree = 100;

    pic->lcd_buf_back = (int *)calloc(pic->width * pic->height, sizeof(int));
    pic->lcd_buf = (int *)calloc(pic->width * pic->height, sizeof(int));

    for (int i = pic->y_start; i < pic->height + pic->y_start; i++)
        for (int j = pic->x_start; j < pic->width + pic->x_start; j++)
            pic->lcd_buf_back[(i - pic->y_start) * pic->width + (j - pic->x_start)] = lcd_get_point(lcd, j, i);

    show_font_to_lcd((int *)lcd->mmap_addr, x, y, canvas);


    for (int i = pic->y_start; i < pic->height + pic->y_start; i++)
        for (int j = pic->x_start; j < pic->width + pic->x_start; j++)
            pic->lcd_buf[(i - pic->y_start) * pic->width + (j - pic->x_start)] = lcd_get_point(lcd, j, i);


    pic->bmp_buf = (char *)calloc(pic->size - pic->offbits, sizeof(char));
    for (int i = 0; i < pic->width * pic->height; i++)
    {
        pic->bmp_buf[i * 4 + 0] = pic->lcd_buf[i] >> 0 & 0xff;
        pic->bmp_buf[i * 4 + 1] = pic->lcd_buf[i] >> 8 & 0xff;
        pic->bmp_buf[i * 4 + 2] = pic->lcd_buf[i] >> 16 & 0xff;
        pic->bmp_buf[i * 4 + 3] = pic->lcd_buf[i] >> 24 & 0xff;
    }

    // 对比图片四个角的颜色
    int *corner = (int *)calloc(4, sizeof(int));
    corner[0] = pic->lcd_buf[0];
    corner[1] = pic->lcd_buf[pic->width * pic->height - 1];
    corner[2] = pic->lcd_buf[pic->width * pic->height - 1 - pic->width];
    corner[3] = pic->lcd_buf[pic->width * pic->height - 1 - pic->width - 1];
    // 只要这四个中有三个相等
    if (corner[0] == corner[1] && corner[0] == corner[2] && corner[0] == corner[3] && canvas->color == TRANSPARENT)
    {
        for (int i = pic->y_start; i < pic->height + pic->y_start; i++)
            for (int j = pic->x_start; j < pic->width + pic->x_start; j++)
                if (lcd_get_point(lcd, j, i) == corner[0])
                    lcd_draw_point(lcd, j, i, pic->lcd_buf_back[(i - pic->y_start) * pic->width + (j - pic->x_start)]);

        for (int i = pic->y_start; i < pic->height + pic->y_start; i++)
            for (int j = pic->x_start; j < pic->width + pic->x_start; j++)
                pic->lcd_buf[(i - pic->y_start) * pic->width + (j - pic->x_start)] = lcd_get_point(lcd, j, i);
    }
    free(corner);
    return pic;
}

void font_close(Font *font)
{
    fontUnload(font);
}

void canvas_close(Canvas *canvas)
{
    destroyBitmap(canvas);
}

