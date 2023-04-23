#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
int main()
{

    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;

    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }
    char tenMay[50];
    short int soDia;
    char tenO;
    short int kichThuoc;
    int pos = 0;
    char buf[256];
    printf("\nTen may tinh:\n");
    scanf("%s", tenMay);
    getchar();

    printf("\nSo o dia: ");
    scanf("%hd", &soDia);
    getchar();
    memcpy(buf, &soDia, sizeof(soDia));
    pos += sizeof(soDia);

    for (int i = 0; i < soDia; i++)
    {

        printf("nhap o dia: ");
        scanf("%c", &tenO);
        buf[pos] = tenO;
        pos++;

        printf("kich thuoc dia:");
        scanf("%hd", &kichThuoc);
        getchar();
        memcpy(buf + pos, &kichThuoc, sizeof(kichThuoc));

        pos += sizeof(kichThuoc);
        // buf[strlen(buf)] = ' ';
    }

    memcpy(buf + pos, tenMay, strlen(tenMay));
    pos += strlen(tenMay);
    buf[pos] = 0;
    // clean_stdin();

    send(client, buf, pos, 0);

    close(client);
    return 0;
}