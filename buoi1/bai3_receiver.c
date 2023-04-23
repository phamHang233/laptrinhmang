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
    // nhập vào câu lệnh theo tham số tên file thực thi - port
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    char namefile[20];
    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);

    int ret1 = recvfrom(receiver, namefile, sizeof(namefile), 0,
                        (struct sockaddr *)&sender_addr, &sender_addr_len);
    printf("%d bytes received %s: %s\n", ret1,
           inet_ntoa(sender_addr.sin_addr), namefile);

    char buf[256];
    // sprintf(buf, "new_%s", namefile);
    FILE *f = fopen(namefile, "wb");
    int ret = recvfrom(receiver, buf, sizeof(buf), 0,
                       (struct sockaddr *)&sender_addr, &sender_addr_len);
    fwrite(buf, 1, ret, f);
    printf("%d byte recv \n", ret);
    printf("%s\n", buf);

    fclose(f);
}