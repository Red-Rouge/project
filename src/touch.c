#include "touch.h"

int touch_init(Touch *touch)
{
    touch->touch_fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
    if (touch->touch_fd == -1)
    {
        perror("open");
        close(touch->touch_fd);
        return -1;
    }
    touch->start = (struct coor *)calloc(1, sizeof(struct coor));
    touch->end = (struct coor *)calloc(1, sizeof(struct coor));
    if (touch->start == NULL || touch->end == NULL)
    {
        perror("calloc");
        close(touch->touch_fd);
        free(touch->start);
        free(touch->end);
        return -1;
    }
    touch->start->x = touch->start->y = 0;
    touch->end->x = touch->end->y = 0;
    touch->status = -1;
    touch->pos = 0;
    touch->buf = (struct input_event *)calloc(1, sizeof(struct input_event));
    touch->time = (struct timeval *)calloc(1, sizeof(struct timeval));
    return 0;
}

Touch *touch_open()
{
    Touch *touch = (Touch *)calloc(1, sizeof(Touch));
    if (touch == NULL)
    {
        perror("calloc");
        return NULL;
    }
    else if (touch_init(touch) == -1)
    {
        perror("touch_info");
        free(touch);
        return NULL;
    }
    return touch;
}




// 关闭触摸屏
int touch_close(Touch *touch)
{
    close(touch->touch_fd);
    free(touch->start);
    free(touch->end);
    free(touch->buf);
    free(touch);
    return 0;
}
//触摸屏信息输出
void touch_info(Touch *touch);


int touch_start(Touch *touch, const unsigned int mode)
{
    touch->start->x = touch->start->y = 0;
    touch->end->x = touch->end->y = 0;
    touch->status = 0;
    touch->pos = 0;
    while (1)
    {
        if (touch_check_timeout(touch) == 0)
        {
            continue;
        }
        if (read(touch->touch_fd, touch->buf, sizeof(*touch->buf)) == -1)
        {
            perror("read");
            return -1;
        }
        // 记录手指按下时的坐标
        if (touch->buf->type == EV_ABS && touch->buf->code == ABS_X)//判断是否发生X轴绝对位移事件
        {
            if (!touch->start->x)
                touch->start->x = touch->buf->value * 800 / 1024;
            touch->end->x = touch->buf->value * 800 / 1024;
        }
        if (touch->buf->type == EV_ABS && touch->buf->code == ABS_Y)//判断是否发生X轴绝对位移事件
        {
            if (!touch->start->y)
                touch->start->y = touch->buf->value * 480 / 600;
            touch->end->y = touch->buf->value * 480 / 600;
        }
        if ((touch->buf->type == EV_KEY && touch->buf->code == BTN_TOUCH && touch->buf->value == 0) &&
            (touch->start->x * touch->start->y * touch->end->x * touch->end->y != 0) && mode == 0)//松手检测
        {
            int tran = touch->end->x - touch->start->x, dire = touch->end->y - touch->start->y;
            // 判断这俩数的绝对值谁大
            if ((abs(tran) > abs(dire)) && abs(tran) > 20)
            {
                touch->status = 1;
                touch->pos = (tran > 0) ? (1) : (-1);
                return 1;
            }
            else if ((abs(dire) > abs(tran)) && abs(dire) > 20)
            {
                touch->status = 1;
                touch->pos = (dire < 0) ? (2) : (-2);
                return 1;
            }
            touch->status = 1;
            return 0;
        }
        else if ((touch->start->x * touch->start->y * touch->end->x * touch->end->y != 0) && mode == 1)
        {
            touch->status = 1;
            return 0;
        }
        else if (touch->start->x != 0 && touch->start->y != 0 && mode == 2)
        {
            touch->status = 1;
            return -1;
        }
    }
    return -1;
}










int touch_clear(Touch *touch)
{
    if (touch == NULL)
        return -1;
    while (touch_check_timeout(touch) != 0 && read(touch->touch_fd, touch->buf, sizeof(*touch->buf)) != -1);
    touch->start->x = touch->start->y = 0;
    touch->end->x = touch->end->y = 0;
    touch->status = 0;
    touch->pos = 0;
    return 0;
}



// 检测查询是否超时
int touch_check_timeout(Touch *touch)
{
    if (touch == NULL)
        return -1;
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(touch->touch_fd, &fds);
    touch->time->tv_sec = 0;
    touch->time->tv_usec = 1000;
    int ret = select(touch->touch_fd + 1, &fds, NULL, NULL, touch->time);
    if (ret == -1)
    {
        perror("select error");
        return -1;
    }
    else if (ret == 0)
    {
        return 0;
    }
    return 1;
}



