#include "2048.h"




Frame **frame_create(int num, Lcd *lcd)
{
    Frame **matrix = (Frame **)malloc(sizeof(Frame *) * num);
    for (int i = 0; i < num; i++)
    {
        matrix[i] = (Frame *)malloc(sizeof(Frame) * num);
        for (int j = 0; j < num; j++)
        {
            matrix[i][j].num = -1;
            matrix[i][j].start = (struct coor *)calloc(1, sizeof(struct coor));
            matrix[i][j].end = (struct coor *)calloc(1, sizeof(struct coor));
            matrix[i][j].start->x = lcd->height * j / num;
            matrix[i][j].start->y = lcd->height * i / num;
            matrix[i][j].end->x = lcd->height * (j + 1) / num;
            matrix[i][j].end->y = lcd->height * (i + 1) / num;
        }
    }
    return matrix;
}

int frame_print(Frame **matrix, Font *font, int num, Lcd *lcd)
{
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            Canvas *canvas = canvas_create(lcd->height / num - 10, lcd->height / num - 10, 4, 0XFFFFEEEE & 0xffff0000 |
                (0XFFFFEEEE & 0xff - (int)(matrix[i][j].num * 3.5)) << 8 |
                0XFFFFEEEE & 0xff - (int)(matrix[i][j].num * 3.5) << 0);
            if (matrix[i][j].num != -1)
                font_printf(font, canvas, canvas->height / 2 - canvas->height / 4, canvas->width / 2 - canvas->width / 4, BLACK, 0, "%d", matrix[i][j].num);
            else
                font_printf(font, canvas, canvas->height / 2, canvas->width / 2, BLACK, 0, "");

            pic_close(canva_show(canvas, lcd, j * lcd->height / num, i * lcd->height / num));
            canvas_close(canvas);
            // printf("%d", matrix[i][j].num);
        }
        // printf("\n");
    }
    return 0;
}

int frame_free(Frame **matrix, int num)
{
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            free(matrix[i][j].start);
            free(matrix[i][j].end);
        }
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}

// 判断是否满了
int frame_fill(Frame **matrix, int num)
{
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            if (matrix[i][j].num == -1)
            {
                return 0;
            }
        }
    }
    return -1;
}
int frame_move(int status, Lcd *lcd)
{
    if (status == -1)
    {
        Font *font_temp = font_open("STCAIYUN.TTF");
        Canvas *canvas = canvas_create(300, 100, 4, BLACK);
        font_size(font_temp, 50);
        font_printf(font_temp, canvas, 110, 30, WHITE, 0, "满了");
        pic_close(canva_show(canvas, lcd, 250, 150));
        canvas_close(canvas);
        font_close(font_temp);
        return -1;
    }
    return 0;
}


int frame_rand(Frame **matrix, int num)
{
    // 随机生成2
    srand((unsigned)time(NULL));
    while (1)
    {
        if (frame_fill(matrix, num) == -1)
        {
            // 无法填充
            return -1;
        }
        int y = rand() % num, x = rand() % num;
        if (matrix[y][x].num == -1)
        {
            matrix[y][x].num = 2;
            return 0;
        }
    }
    return -1;
}









int matrix_processing(Frame **matrix, int num, Touch *touch)
{
    int status = 0;
    if (matrix == NULL || touch == NULL)
    {
        printf("无法打开设备\n");
        return -1;
    }
    // Frame **matrix_new = frame_create(num, lcd);
    if (touch->pos == -1)
    {
        for (int i = 0; i < num; i++)
        {
            int writeIndex = 0;
            for (int j = 0; j < num; j++)
            {
                if (matrix[i][j].num != -1)
                {
                    if (writeIndex != j)
                    {
                        status = 1;
                        matrix[i][writeIndex].num = matrix[i][j].num;
                        matrix[i][j].num = -1;
                    }
                    writeIndex++;
                }
            }
            for (int j = 0; j < num - 1; j++)
            {
                if ((matrix[i][j].num == matrix[i][j + 1].num && matrix[i][j].num != -1) && matrix[i][j].num == matrix[i][j + 1].num != 64)
                {
                    status = 1;
                    matrix[i][j].num *= 2;
                    matrix[i][j + 1].num = -1;
                }
            }
        }

    }
    else if (touch->pos == 1)
    {
        for (int i = 0; i < num; i++)
        {
            int writeIndex = num - 1;
            for (int j = num - 1; j >= 0; j--)
            {
                if (matrix[i][j].num != -1)
                {
                    if (writeIndex != j)
                    {
                        status = 1;
                        matrix[i][writeIndex].num = matrix[i][j].num;
                        matrix[i][j].num = -1;
                    }
                    writeIndex--;
                }
            }
            for (int j = num - 1; j > 0; j--)
            {
                if ((matrix[i][j].num == matrix[i][j + 1].num && matrix[i][j].num != -1) && matrix[i][j].num == matrix[i][j + 1].num != 64)
                {
                    status = 1;
                    matrix[i][j].num *= 2;
                    matrix[i][j - 1].num = -1;
                }
            }
        }
    }
    else if (touch->pos == 2)
    {
        for (int j = 0; j < num; j++)
        {
            int writeIndex = 0;
            for (int i = 0; i < num; i++)
            {
                if (matrix[i][j].num != -1)
                {
                    if (writeIndex != i)
                    {
                        status = 1;
                        matrix[writeIndex][j].num = matrix[i][j].num;
                        matrix[i][j].num = -1;
                    }
                    writeIndex++;
                }
            }
            for (int i = 0; i < num - 1; i++)
            {
                if ((matrix[i][j].num == matrix[i + 1][j].num && matrix[i][j].num != -1) && matrix[i][j].num == matrix[i][j + 1].num != 64)
                {
                    status = 1;
                    matrix[i][j].num *= 2;
                    matrix[i + 1][j].num = -1;
                }
            }
        }
    }
    else if (touch->pos == -2)
    {
        for (int j = 0; j < num; j++)
        {
            int writeIndex = num - 1;
            for (int i = num - 1; i >= 0; i--)
            {
                if (matrix[i][j].num != -1)
                {
                    if (writeIndex != i)
                    {
                        status = 1;
                        matrix[writeIndex][j].num = matrix[i][j].num;
                        matrix[i][j].num = -1;
                    }
                    writeIndex--;
                }
            }
            for (int i = num - 1; i > 0; i--)
            {
                if ((matrix[i][j].num == matrix[i - 1][j].num && matrix[i][j].num != -1) && matrix[i][j].num == matrix[i][j + 1].num != 64)
                {
                    status = 1;
                    matrix[i][j].num *= 2;
                    matrix[i - 1][j].num = -1;
                }
            }
        }
    }
    int test1 = (frame_rand(matrix, num) == -1), test2 = (frame_rand(matrix, num) == -1);
    return ((test1 || test2) && status == 0) ? -1 : 0;
}













void game(Lcd *lcd, Stack *stack, Touch *touch, int num)
{
    int status = 0;
    if (lcd == NULL || touch == NULL)
    {
        printf("无法打开设备\n");
        return;
    }
    lcd_clear(lcd, WHITE);
    Frame **matrix = frame_create(num, lcd);
    Font *font = font_open("STCAIYUN.TTF");
    font_size(font, lcd->height / num / 2);
    frame_rand(matrix, num);
    frame_print(matrix, font, num, lcd);
    while (1)
    {
        touch_start(touch, 0);
        if (touch->status == 1)
        {
            if (touch->start->x > 750 && touch->start->y < 50)
                break;
            if (touch->pos != 0)
            {
                // 矩阵整合
                if (status == -1)
                    lcd_clear(lcd, WHITE);
                status = matrix_processing(matrix, num, touch);
            }
            else
            {
                touch_clear(touch);
                continue;
            }
            frame_print(matrix, font, num, lcd);
            frame_move(status, lcd);
            touch_clear(touch);
        }
    }
    font_close(font);
    frame_free(matrix, num);
    pic_show(stack_top(stack)->pic, lcd, 0, 0);
}




