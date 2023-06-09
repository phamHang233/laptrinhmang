#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Sai tham so URL.\n");
        return 1;
    }
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

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
    printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // luu dia chi IP va cong vao file
    FILE *f = fopen(argv[2], "a");
    char *clientAddrStr = inet_ntoa(clientAddr.sin_addr);
    fwrite(clientAddrStr, 1, strlen(clientAddrStr), f);
    // printf("%s - %ld- %ld ", clientAddrStr, sizeof(clientAddrStr), strlen(clientAddrStr));
    fwrite("\n", 1, 1, f);
    char buf[26];
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);

        if (ret <= 0)
        {
            printf("Ket noi bi dong.\n");
            break;
        }

        if (ret < sizeof(buf))
            buf[ret] = 0;
        fwrite(buf, 1, ret, f);
        printf("%d bytes received\n", ret);
        printf("%s\n", buf);
    }
    fclose(f);
    close(client);
    close(listener);
    return 0;
}