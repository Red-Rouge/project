#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

#include "lcd.h"
#include "pic.h"
#include "link.h"
#include "touch.h"
#include "stack.h"
#include "font.h"
#include "2048.h"

int findCFiles(Link *head, const char *path);
void scratch(Lcd *lcd, Stack *stack, Touch *touch); //刮刮乐
int album(Lcd *lcd, Stack *stack, Touch *touch); //电子相册
int wooden_fish(Lcd *lcd, Stack *stack, Touch *touch); //木鱼




int main()
{
    Lcd *lcd = lcd_open();
    Stack *stack = stack_create(NULL);
    Touch *touch = touch_open();
    Pic *main = pic_new("./pic/main/main.bmp");

    stack_push(stack, stack_create(pic_show(main, lcd, 0, 0)));

    while (1)
    {
        touch_start(touch, 0);
        if (touch->status == 1)
        {
            /* code */
            if (touch->start->x > 0 && touch->start->y > 160 &&
                touch->end->x < 160 && touch->start->y < 320)
            {
                wooden_fish(lcd, stack, touch);
            }
            else if (touch->start->x > 160 && touch->start->y > 160 &&
                touch->end->x < 320 && touch->start->y < 320)
            {
                album(lcd, stack, touch);
            }
            else if (touch->start->x > 320 && touch->start->y > 160 &&
                touch->end->x < 480 && touch->start->y < 320)
            {
                // 随机生成3~8的数字
                srand(time(NULL));
                game(lcd, stack, touch, rand() % 6 + 3);
            }
            else if (touch->start->x > 480 && touch->start->y > 160 &&
                touch->end->x < 640 && touch->start->y < 320)
            {
                scratch(lcd, stack, touch);
            }
            else if (touch->start->x > 750 && touch->start->y < 50)
                break;
            touch_clear(touch);
        }

    }
    pic_close(main);
    lcd_clear(lcd, WHITE);
    stack_clear(stack);
    touch_close(touch);
    lcd_close(lcd);
    return 0;
}


























void scratch(Lcd *lcd, Stack *stack, Touch *touch)
{
    if (lcd == NULL || touch == NULL)
    {
        printf("无法打开设备\n");
        return;
    }
    lcd_clear(lcd, WHITE);
    Stack *pic_stack = stack_create(NULL);
    Font *font = font_open("STCAIYUN.TTF");
    font_size(font, 70);


    char *prize[23] = { "1","5","10","20","50","88","100","200","500","888","1K","2K","5K","8888","1W","2W","5W","88888","10W","20W","50W","888888","100W" };
    int num = 0;
    // 随机乘胜1~1000000的数字
    srand(time(NULL));
    num = rand() % 23;

    Canvas *canvas = canvas_create(300, 150, 4, TRANSPARENT);
    font_printf(font, canvas, canvas->width / 2 - 70 * strlen(prize[num]) / 4, canvas->height / 2 - 70 / 2, YELLOW, 0, "%s", prize[num]);
    Pic *pic1 = canva_show(canvas, lcd, 250, 100);
    stack_push(pic_stack, stack_create(pic1));
    canvas_close(canvas);

    canvas = canvas_create(300, 150, 4, BLACK);
    font_printf(font, canvas, 10, 10, BLACK, 0, "");
    Pic *pic2 = canva_show(canvas, lcd, 250, 100);
    stack_push(pic_stack, stack_create(pic2));
    canvas_close(canvas);



    int status = 1;
    while (1)
    {
        touch_start(touch, status == 1 ? 1 : 0);
        if (touch->start->x > 750 && touch->start->y < 50)
            break;
        if (touch->status == 1 && status == 1)
        {
            pic_draw_circle(lcd, touch->start->x, touch->start->y, 20, stack_top(pic_stack)->pic);
            if (stack_top(pic_stack)->pic->display_degree < 10)
            {
                pic_clear(stack_pop(pic_stack)->pic, lcd);
                touch_clear(touch);
                status = 0;
            }
            touch_clear(touch);
        }
    }
    font_close(font);
    stack_clear(pic_stack);
    pic_close(pic1);
    pic_close(pic2);
    pic_show(stack_top(stack)->pic, lcd, 0, 0);
}


int album(Lcd *lcd, Stack *stack, Touch *touch)
{
    if (lcd == NULL || touch == NULL)
    {
        printf("无法打开设备\n");
        return -1;
    }

    lcd_clear(lcd, WHITE);
    Link *head = link_create(NULL), *cur = head;
    if (findCFiles(head, "./pic/album") == -1)
    {
        printf("无法打开目录\n");
        return -1;
    }

    cur = cur->next;
    pic_show(cur->pic, lcd, 0, 0);
    // pic_info(cur->pic);
    // printf("-------------------\n");
    while (1)
    {
        touch_start(touch, 0);
        if (touch->status == 1)
        {
            if (touch->pos == 1)
            {
                cur = (cur->next == head) ? cur->next->next : cur->next;
                pic_show(cur->pic, lcd, 0, 0);
                // pic_info(cur->pic);
                // printf("-------------------\n");
            }
            else if (touch->pos == -1)
            {
                cur = (cur->prev == head) ? cur->prev->prev : cur->prev;
                pic_show(cur->pic, lcd, 0, 0);
                // pic_info(cur->pic);
                // printf("-------------------\n");
            }
            if (touch->start->x > 750 && touch->start->y < 50)
                break;
            touch_clear(touch);
        }
    }
    // LCD清屏
    // 关闭LCD
    // 释放链表
    link_free(head);
    pic_show(stack_top(stack)->pic, lcd, 0, 0);
    return 0;
}











int wooden_fish(Lcd *lcd, Stack *stack, Touch *touch) //木鱼
{
    if (lcd == NULL || touch == NULL)
    {
        printf("无法打开设备\n");
        return -1;
    }
    FILE *file = fopen("./app/wooden_fish/merits_and_virtues.txt", "a");
    fclose(file);
    file = fopen("./app/wooden_fish/merits_and_virtues.txt", "r+"); // 以读写模式打开文件
    if (file == NULL)
    {
        printf("无法打开文件\n");
        return 1;
    }

    // 计算文件长度
    fseek(file, 0, SEEK_END); // 将文件指针定位到文件末尾
    int length = ftell(file); // 获取文件指针当前位置相对于文件开头的偏移字节数
    fseek(file, 0, SEEK_SET); // 将文件指针重新定位到文件开头
    if (length == 0)
    {
        fprintf(file, "%d", 0);
    }
    int number;
    fseek(file, 0, SEEK_SET); // 将文件指针重新定位到文件开头
    fscanf(file, "%d", &number); // 从文件中读取数字

    lcd_clear(lcd, WHITE);
    Pic *wait = pic_new("./app/wooden_fish/wait.bmp");
    Pic *click = pic_new("./app/wooden_fish/click.bmp");

    pic_show(wait, lcd, 0, 0);

    Font *font = font_open("./STCAIYUN.TTF");
    font_size(font, 40);

    Canvas *canvas_test = canvas_create(400, 50, 4, TRANSPARENT);
    font_printf(font, canvas_test, 0, 0, YELLOW, 0, "  功德:%d", number);
    Pic *pic_test = canva_show(canvas_test, lcd, 300, 400);
    canvas_close(canvas_test);


    while (1)
    {
        touch_start(touch, 2);
        if (touch->status == 1)
        {
            number++;
            if (touch->start->x > 750 && touch->start->y < 50)
                break;

            pic_show(click, lcd, 0, 0);
            Canvas *canvas = canvas_create(200, 50, 4, TRANSPARENT);
            font_printf(font, canvas, 0, 0, YELLOW, 0, "  功德+1");
            Pic *pic = canva_show(canvas, lcd, 300, 20);
            canvas_close(canvas);
            pic_show(pic_test, lcd, pic_test->x_start, pic_test->y_start);
            usleep(50 * 1000);
            pic_clear(pic, lcd);
            pic_close(pic);
            touch_clear(touch);



            pic_show(wait, lcd, 0, 0);


            pic_clear(pic_test, lcd);
            pic_close(pic_test);
            canvas_test = canvas_create(400, 50, 4, TRANSPARENT);
            font_printf(font, canvas_test, 0, 0, YELLOW, 0, "  功德:%d", number);
            fseek(file, 0, SEEK_SET); // 将文件指针重新定位到文件开头
            fprintf(file, "%d", number); // 将新的数字写回文件
            pic_test = canva_show(canvas_test, lcd, 300, 400);
            canvas_close(canvas_test);
        }

    }
    pic_close(pic_test);
    fclose(file); // 关闭文件
    font_close(font);
    pic_close(wait);
    pic_close(click);
    lcd_clear(lcd, WHITE);
    pic_show(stack_top(stack)->pic, lcd, 0, 0);
    return 0;
}









int findCFiles(Link *head, const char *path)
{
    // 打开目录
    DIR *dir;
    // 读取目录中的每一个文件
    struct dirent *entry;
    // 获取文件信息
    struct stat fileStat;
    // 拼接文件路径
    char filePath[512];

    // 打开目录
    if (!(dir = opendir(path)))
    {
        printf("无法打开目录: %s\n", path);
        return -1;
    }

    // 遍历目录中的每一个文件
    while ((entry = readdir(dir)) != NULL)
    {
        // 跳过 . 和 ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        // 拼接文件路径
        snprintf(filePath, sizeof(filePath), "%s/%s", path, entry->d_name);
        // 获取文件信息
        if (stat(filePath, &fileStat) < 0)
            continue;
        if (S_ISDIR(fileStat.st_mode))
        {
            // 递归调用，进入子目录
            findCFiles(head, filePath);
        }
        else
        {
            // 如果文件是.canvasp格式，则添加到链表中
            if ((strstr(entry->d_name, ".bmp") != NULL))
            {
                link_add_tail(head, link_create(pic_new(filePath)));
            }
        }
    }
    // 关闭目录
    closedir(dir);
    return 0;
}



