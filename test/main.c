#include <stdio.h>

int main()
{


    FILE *file = fopen("text.txt", "a");
    fclose(file);

    file = fopen("text.txt", "r+"); // 以读写模式打开文件

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
    number++; // 将数字加1

    fseek(file, 0, SEEK_SET); // 将文件指针重新定位到文件开头
    fprintf(file, "%d", number); // 将新的数字写回文件

    fclose(file); // 关闭文件

    return 0;
}
