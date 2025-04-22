#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define FILENAME "time_log.txt"
time_t timegm(struct tm *tm);

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

// 写线程：每秒写入当前时间到文件
void *write_time(void *arg) {
    FILE *fp;
    time_t rawtime;
    struct tm *timeinfo;

    while (1) {
        pthread_mutex_lock(&file_mutex);
        fp = fopen(FILENAME, "a");
        if (fp == NULL) {
            perror("Failed to open file");
            pthread_mutex_unlock(&file_mutex);
            continue;
        }

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d\n",
            (int)(timeinfo->tm_year + 1900), (int)(timeinfo->tm_mon + 1), (int)timeinfo->tm_mday,
            (int)timeinfo->tm_hour, (int)timeinfo->tm_min, (int)timeinfo->tm_sec);
        fclose(fp);
        pthread_mutex_unlock(&file_mutex);
        sleep(1);
    }
    return NULL;
}

// 读线程：读取文件内容并转换为UTC时间戳
void *read_and_print(void *arg) {
    FILE *fp;
    char buffer[100];
    struct tm timeinfo;

    while (1) {
        sleep(2);
        pthread_mutex_lock(&file_mutex);
        fp = fopen(FILENAME, "r+");
        if (fp == NULL) {
            perror("Failed to open file");
            pthread_mutex_unlock(&file_mutex);
            continue;
        }

        printf("current contents:\n");
        while (fgets(buffer, sizeof(buffer), fp)){
            if (strptime(buffer, "%Y-%m-%d %H:%M:%S", &timeinfo) != NULL){
                time_t utc_time = timegm(&timeinfo);
                printf("UTC Timestamp: %ld\n", utc_time);
            }
        }
        
        ftruncate(fileno(fp), 0);
        fclose(fp);
        pthread_mutex_unlock(&file_mutex);
    }
    return NULL;
}

int main(){
    pthread_t writer, reader;

    pthread_create(&writer, NULL, write_time, NULL);
    pthread_create(&reader, NULL, read_and_print, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);
    return 0;
}