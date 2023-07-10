#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

int users[64];
int num_users = 0;
char userName[64][32];
int op = -1;
void *client_thread(void *);

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
    addr.sin_port = htons(9000);

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

    while (1)
    {
        int client = accept(listener, NULL, NULL);
        if (client == -1)
        {
            perror("accept() failed");
            continue;
            ;
        }
        printf("New client connected: %d\n", client);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, client_thread, &client);
        pthread_detach(thread_id);
    }

    close(listener);

    return 0;
}

void *client_thread(void *param)
{
    int client = *(int *)param;

    char buf[256];
    char cmd[32], name[32], msg_cmd[128], tmp[32];

    int isLog = 0;

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;

        buf[ret] = 0;
        printf("Received from %d: %s\n", client, buf);

        if (!isLog)
        {
            // Xu ly cu phap lenh dang nhap

            ret = sscanf(buf, "%s%s%s", cmd, name, tmp);
            if (ret == 2)
            {
                if (strcmp(cmd, "JOIN") == 0)
                {
                    int i;
                    if (num_users == 0)
                    {
                        char *msg = "100 OK\n";
                        send(client, msg, strlen(msg), 0);

                        // Luu vao mang user
                        users[num_users] = client;
                        strcpy(userName[num_users], name);
                        op = 0;
                        num_users++;
                        isLog = 1;
                    }
                    for (i = 0; i < num_users; i++)
                    {
                        if (strcmp(name, userName[i]) == 0)
                        {
                            char *msg = "200 NICKNAME IN USE\n";
                            send(client, msg, strlen(msg), 0);
                            break;
                        }
                    }
                    if (i == num_users)
                    {
                        char *msg = "100 OK\n";
                        send(client, msg, strlen(msg), 0);

                        // Luu vao mang user
                        users[num_users] = client;
                        strcpy(userName[num_users], name);
                        num_users++;
                        isLog = 1;
                    }
                }
                else
                {
                    char *msg = "ERR Sai cu phap. Hay nhap lai.\n";
                    send(client, msg, strlen(msg), 0);
                }
            }
            else
            {
                char *msg = "ERR Sai tham so. Hay nhap lai.\n";
                send(client, msg, strlen(msg), 0);
            }
        }
        else
        {
            ret = sscanf(buf, "%s%s%s", cmd, msg_cmd, tmp);
            if (ret == 2)
            {
                if (strcmp(cmd, "MSG") == 0)
                {
                    char sendbuf[256];

                    strcpy(sendbuf, name);
                    strcat(sendbuf, ": ");
                    strcat(sendbuf, buf);

                    // Forward du lieu cho cac user
                    for (int k = 0; k < num_users; k++)
                        if (users[k] != client)
                            send(users[k], sendbuf, strlen(sendbuf), 0);
                }
            }
            // nếu là chủ phong chat
            if (client == op)
            {
                ret = sscanf(buf, "%s%s%s", cmd, msg_cmd, tmp);
                if (ret == 2)
                {
                    // chủ phòng chat thực hiện chuyển quyền
                    if (strcmp(cmd, "OP") == 0)
                    {
                        for (int i = 0; i < num_users; i++)
                        {
                            if (strcmp(msg_cmd, userName[i]) == 0)
                            {
                                char *msg = "100 OK";
                                send(client, msg, strlen(msg), 0);
                                op = i;
                            }
                        }
                    }
                }
            }
        }
    }

    // remove client

    close(client);
}