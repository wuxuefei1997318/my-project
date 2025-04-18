#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

int main() {
    FILE *fp;
    char buffer[BUFFER_SIZE];

    // 创建并打开文件
    fp = fopen("exi.txt", "w+");
    if (fp == NULL) {
        perror("无法打开文件");
        return EXIT_FAILURE;
    }

    // 写入文件
    fprintf(fp, "使用标准 I/O \n");
    fputs("这是另一库函数来实现创建文件、打开文件、写入文件、读取文件和关闭文件的功能。\n", fp);

    // 重置文件指针到文件开头
    rewind(fp);

    // 读取文件
    printf("文件内容：\n");
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        printf("%s", buffer);
    }

    // 关闭文件
    fclose(fp);

    return EXIT_SUCCESS;
}