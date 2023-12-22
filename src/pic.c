#include "pic.h"
// BMP格式头规范
struct bitmap_header
{
    int16_t type;
    int32_t size; // 图像文件大小
    int16_t reserved1;
    int16_t reserved2;
    int32_t offbits; // bmp图像数据偏移量
}__attribute__((packed));

struct bitmap_info
{
    int32_t size;   // 本结构大小	
    int32_t width;  // 图像宽
    int32_t height; // 图像高
    int16_t planes;

    int16_t bit_count; // 色深
    int32_t compression;
    int32_t size_img; // bmp数据大小，必须是4的整数倍
    int32_t X_pel;
    int32_t Y_pel;
    int32_t clrused;
    int32_t clrImportant;
}__attribute__((packed));




int pic_init(Pic *pic, char *path)
{
    // 打开文件
    pic->pic_fd = open(path, O_RDONLY);
    if (pic->pic_fd < 0)
    {
        perror("open");
        return -1;
    }
    // 读取文件路径
    pic->path = (char *)calloc(strlen(path) + 1, sizeof(char));
    strcpy(pic->path, path);
    // 读取文件头
    struct bitmap_header head;
    struct bitmap_info info;
    if (read(pic->pic_fd, &head, sizeof(head)) == -1 || read(pic->pic_fd, &info, sizeof(info)) == -1)
    {
        perror("read");
        return -1;
    }
    // 读取文件头
    pic->size = head.size;
    pic->offbits = head.offbits;
    pic->width = info.width;
    pic->height = info.height;
    pic->bit_count = info.bit_count;
    pic->size_img = info.size_img;

    // 设置显示范围
    pic->x_start = 0;
    pic->y_start = 0;
    pic->display_degree = 0;
    // 读取文件数据
    pic->bmp_buf = (char *)calloc(pic->size - pic->offbits, sizeof(char));
    lseek(pic->pic_fd, pic->offbits, SEEK_SET);
    if (read(pic->pic_fd, pic->bmp_buf, pic->size - pic->offbits) == -1)
    {
        perror("read");
        return -1;
    }
    // 将bmp_buf整合到lcd_buf
    pic->lcd_buf_back = (int *)calloc(pic->width * pic->height, sizeof(int));
    pic->lcd_buf = (int *)calloc(pic->width * pic->height, sizeof(int));
    for (int i = 0; i < pic->width * pic->height; i++)
    {
        pic->lcd_buf[i] = pic->bmp_buf[i * 4 + 3] << 24 | pic->bmp_buf[i * 4 + 2] << 16 | pic->bmp_buf[i * 4 + 1] << 8 | pic->bmp_buf[i * 4];
    }
    // 图片数据上下反转
    int *reversed_buf = (int *)calloc(pic->width * pic->height, sizeof(int));
    for (int y = 0; y < pic->height; y++)
    {
        for (int x = 0; x < pic->width; x++)
        {
            reversed_buf[(pic->height - 1 - y) * pic->width + x] = pic->lcd_buf[y * pic->width + x];
        }
    }
    // 将reversed_buf 给lcdbuf
    free(pic->lcd_buf);
    pic->lcd_buf = reversed_buf;

    // 将lcdbuf的那日容还给bmp_buf
    for (int i = 0; i < pic->width * pic->height; i++)
    {
        pic->bmp_buf[i * 4 + 0] = pic->lcd_buf[i] >> 0 & 0xff;
        pic->bmp_buf[i * 4 + 1] = pic->lcd_buf[i] >> 8 & 0xff;
        pic->bmp_buf[i * 4 + 2] = pic->lcd_buf[i] >> 16 & 0xff;
        pic->bmp_buf[i * 4 + 3] = pic->lcd_buf[i] >> 24 & 0xff;
    }
    return 0;
}



Pic *pic_new(char *path)
{
    Pic *pic = malloc(sizeof(Pic));
    if (pic_init(pic, path) == -1)
    {
        perror("pic_init");
        free(pic);
        return NULL;
    }
    return pic;
}










int pic_close(Pic *pic)
{
    // 如果文件描述符大于等于0，则关闭文件
    if (pic->pic_fd >= 0)
    {
        close(pic->pic_fd);
        pic->pic_fd = -1;
    }
    // 释放路径指针
    free(pic->path);
    // 释放lcd_buf指针
    free(pic->lcd_buf);
    // 释放lcd_buf_back指针
    free(pic->lcd_buf_back);
    free(pic->bmp_buf);
    // 释放pic指针
    free(pic);
    return 0;
}
Pic *pic_show(Pic *pic, Lcd *lcd, int x, int y)
{
    // 如果图片的起始位置大于屏幕的宽度和高度，则返回0
    if (pic->x_start > lcd->width || pic->y_start > lcd->height)
        return NULL;
    // 设置图片的起始位置
    pic->x_start = x;
    pic->y_start = y;
    // 读取底部颜色
    for (int i = pic->y_start; i < pic->height + pic->y_start; i++)
        for (int j = pic->x_start; j < pic->width + pic->x_start; j++)
            pic->lcd_buf_back[(i - pic->y_start) * pic->width + (j - pic->x_start)] = lcd_get_point(lcd, j, i);
    // // 绘制图片
    if (x == 0 && y == 0 && pic->width == lcd->width && pic->height == lcd->height)
    {
        // 将pic->bmp_buf赋值给lcd->mmap_addr
        memcpy(lcd->mmap_addr, pic->bmp_buf, pic->width * pic->height * 4);
    }
    else
        for (int i = pic->y_start; i < pic->height + pic->y_start; i++)
            for (int j = pic->x_start; j < pic->width + pic->x_start; j++)
                lcd_draw_point(lcd, j, i, pic->lcd_buf[(i - pic->y_start) * pic->width + (j - pic->x_start)]);

    pic->display_degree = 100;
    return pic;
}
int pic_clear(Pic *pic, Lcd *lcd)
{
    // 用底色填充
    for (int i = pic->y_start; i < pic->height + pic->y_start; i++)
        for (int j = pic->x_start; j < pic->width + pic->x_start; j++)
            lcd_draw_point(lcd, j, i, pic->lcd_buf_back[(i - pic->y_start) * pic->width + (j - pic->x_start)]);
    pic->x_start = pic->width;
    pic->y_start = pic->height;
    pic->display_degree = 0;
    return 0;
}


void pic_draw_circle(Lcd *lcd, int centerX, int centerY, int radius, Pic *pic)
{
    float area = 0;
    float pic_area = pic->width * pic->height;
    for (int y = ((centerY - radius) < 0 ? 0 : (centerY - radius));y <= ((centerY + radius) > lcd->width ? lcd->height - 1 : (centerY + radius));y++)
    {
        for (int x = ((centerX - radius) < 0 ? 0 : (centerX - radius));x <= ((centerX + radius) > lcd->width ? lcd->width - 1 : (centerX + radius));x++)
        {
            if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius)
            {
                if (pic->lcd_buf_back[(y - pic->y_start) * pic->width + (x - pic->x_start)] == lcd_get_point(lcd, x, y))
                    area -= 1;
                if (x >= pic->x_start && y >= pic->y_start && x < pic->x_start + pic->width && y < pic->y_start + pic->height)
                {
                    lcd_draw_point(lcd, x, y, pic->lcd_buf_back[(y - pic->y_start) * pic->width + (x - pic->x_start)]);
                    area += 1;
                }

            }
        }
    }
    // 计算圆的面积
    pic->display_degree -= (area >= 0 ? area : 0) * 103.555 / pic_area;
}








int pic_info(Pic *pic)
{
    // 判断pic_fd是否小于0
    if (pic->pic_fd < 0)
    {
        // 打印错误信息
        perror("pic_show");
        // 返回-1
        return -1;
    }
    // 打印提示信息
    printf("pic_show\n");
    // 打印pic的路径
    printf("path: %s\n", pic->path);
    // 打印pic的大小
    printf("size: %d\n", pic->size);
    // 打印pic的偏移量
    printf("offbits: %d\n", pic->offbits);
    // 打印pic的宽度
    printf("width: %d\n", pic->width);
    // 打印pic的高度
    printf("height: %d\n", pic->height);
    // 打印pic的位计数
    printf("bit_count: %d\n", pic->bit_count);
    // 打印pic的图片大小
    printf("size_img: %d\n", pic->size_img);
    // 返回0
    return 0;
}














