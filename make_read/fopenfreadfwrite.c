#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 256

void stdio_file_operations() {
    printf("\n=== 使用标准I/O库函数 ===\n");
    
    FILE *fp = fopen("example_stdio.txt", "w+");
    if (!fp) {
        fprintf(stderr, "错误：无法打开文件 'example_stdio.txt' (%s)\n", strerror(errno));
        return;
    }

    const char *text = "使用标准I/O库写入的文本\n";
    size_t text_len = strlen(text);
    if (fwrite(text, 1, text_len, fp) != text_len) {
        fprintf(stderr, "错误：写入不完整 (%s)\n", strerror(errno));
        fclose(fp);
        return;
    }

    int num = 42;
    double pi = 3.14159;
    if (fprintf(fp, "格式化写入：%d %.2f\n", num, pi) < 0) {
        fprintf(stderr, "错误：格式化写入失败 (%s)\n", strerror(errno));
        fclose(fp);
        return;
    }
    printf("-> 写入完成\n");

    rewind(fp);
    char buffer[BUFFER_SIZE];
    printf("\n读取内容：\n");
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }

    if (ferror(fp)) {
        fprintf(stderr, "错误：文件读取错误 (%s)\n", strerror(errno));
    }
    printf("-> 读取完成\n");

    if (fclose(fp) != 0) {
        fprintf(stderr, "警告：文件关闭失败 (%s)\n", strerror(errno));
    }
}

void posix_file_operations() {
    printf("\n=== 使用POSIX系统调用 ===\n");
    
    int fd = open("example_posix.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "错误：无法打开文件 'example_posix.txt' (%s)\n", strerror(errno));
        return;
    }

    const char *text = "使用系统调用写入的文本\n";
    ssize_t text_len = (ssize_t)strlen(text);
    ssize_t total_written = 0;
    while (total_written < text_len) {
        ssize_t wrote = write(fd, text + total_written, text_len - total_written);
        if (wrote == -1) {
            fprintf(stderr, "错误：写入失败 (%s)\n", strerror(errno));
            close(fd);
            return;
        }
        total_written += wrote;
    }
    printf("-> 写入完成\n");

    if (lseek(fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "错误：文件定位失败 (%s)\n", strerror(errno));
        close(fd);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes;
    printf("\n读取内容：\n");
    while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }
    if (bytes == -1) {
        fprintf(stderr, "错误：文件读取失败 (%s)\n", strerror(errno));
    }
    printf("-> 读取完成\n");

    if (close(fd) == -1) {
        fprintf(stderr, "警告：文件关闭失败 (%s)\n", strerror(errno));
    }
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    stdio_file_operations();
    posix_file_operations();
    return EXIT_SUCCESS;
}