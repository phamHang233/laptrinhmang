// C program to demonstrate snprintf()
#include <stdio.h>

int main()
{
       FILE *f = fopen("tcp_hello.txt", "r");
       char buf[128];
       int r;
       while (r = fgets(buf, 128, f) != NULL)
       {
              // Xuất từng dòng ra màn hình
              printf("%ld", strlen(buf));
       }
       return 0;
}
