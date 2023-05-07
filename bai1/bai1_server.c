#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((9000));

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5))
    {
        perror("listen() failed");
        return 1;
    }

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(addr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    char *clientAddrStr = inet_ntoa(clientAddr.sin_addr);

    char buf[256];
    char tenMay[50];
    short int soDia;
    char tenO;
    short int kichThuoc;

    int ret = recv(client, buf, sizeof(buf), 0);
    buf[ret] = 0;
    printf("%d bytes received\n", ret);
    // so o Dia duoc luu o vi tri dau tien
    memcpy(&soDia, buf, sizeof(soDia));
    // ten May:
    strncpy(tenMay, buf + 3 * soDia + 2, ret - (3 * soDia + 2));
    printf("Ten may tinh: %s\n", tenMay);

    printf("So o dia: %d\n", soDia);

    for (int i = 0; i < soDia; i++)
    {
        tenO = buf[3 * i + 2];

        memcpy(&kichThuoc, buf + 3 * i + 3, sizeof(kichThuoc));
        printf("Ten o: %c - Kich thuoc o: %hdGB\n", tenO, kichThuoc);
    }

    close(client);
    close(listener);
    return 0;
}