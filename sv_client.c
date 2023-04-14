#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Sai tham so URL\n");
        return 0;
    }
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;

    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }
    char buf[256];
    printf("\nNhap thong tin sinh vien\n");

    printf("\nNhap MSSV: ");
    fgets(buf, sizeof(buf), stdin);
    send(client, buf, strlen(buf), 0);

    printf("\nNhap Ho ten: ");
    fgets(buf, sizeof(buf), stdin);
    send(client, buf, strlen(buf), 0);

    printf("\nNhap Ngay sinh: ");
    fgets(buf, sizeof(buf), stdin);
    send(client, buf, strlen(buf), 0);

    printf("\nNhap Diem trung binh: ");
    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        if (strncmp("exit", buf, 4) == 0)
        {
            break;
        }
        send(client, buf, strlen(buf), 0);
    }
    // thoi gian client gui di
    time_t rawtime;
    struct tm *time_str;
    time(&rawtime);
    time_str = localtime(&rawtime);

    snprintf(buf, sizeof(buf), "%d-%d-%d %d:%d:%d",
             time_str->tm_mday, time_str->tm_mon + 1, time_str->tm_year + 1900,
             time_str->tm_hour, time_str->tm_min, time_str->tm_sec);
    send(client, buf, strlen(buf), 0);

    close(client);
    return 0;
}