#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <pthread.h>

int soc,acc;
struct sockaddr_in info, remote;
char filenameString[10];
char buffer[100];
char tempChar;
char length;

void main()
{
    int one = 1;
    socklen_t sone;

    sone = sizeof(one);
    soc = socket(AF_INET, SOCK_STREAM, 0);
    info.sin_addr.s_addr = INADDR_ANY;
    info.sin_family = AF_INET;
    info.sin_port = htons(8000);
    setsockopt(soc, SOL_SOCKET, SO_REUSEPORT, &one, sone);
    bind(soc, (struct sockaddr*)&info, sizeof(info));


    listen(soc, 2);
    socklen_t isz = sizeof(struct sockaddr);
    acc = accept(soc, (struct sockaddr*)&remote, &isz);
    printf("Connected!\n");
    FILE *in;
    recv(acc, filenameString, 10, 0);
    while(1)
    {
        bzero(filenameString, 10);
        recv(acc, filenameString, 10, 0);
        printf("RECVD %s\n", filenameString);
        in = fopen(filenameString, "r");
        length = 0;
        while(((tempChar = fgetc(in)) != EOF) && length < 99)
            buffer[length++] = tempChar;
        write(acc, buffer,length);
        fclose(in);
    }
}
