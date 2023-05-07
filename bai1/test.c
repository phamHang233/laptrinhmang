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
    // int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // //-------------------------
    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET;

    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // addr.sin_port = htons(9000);
    // //--------------------------------

    // int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    // if (ret == -1)
    // {
    //     perror("connect() failed");
    //     return 1;
    // }

    char buf[32];
    FILE *f = fopen("0to9file.txt", "r");
    char s[] = "abcd";
    char a;
    while ((a = fgetc(f)) != EOF)
    {
        printf("%c", a);
    }

    // printf("%s", buf);
    // while ( != NULL)
    // {
    //     send(client, buf, strlen(buf), 0);
    // }
    // char s[] = "abcd";
    // send(client, s, strlen(s), 0);
    fclose(f);

    // close(client);
}