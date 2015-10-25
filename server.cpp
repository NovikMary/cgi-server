#include <iostream>
//#include "unp.h"
//#include "error.c"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MAXLINE  4096

using namespace std;



int main(int argc, char **argv) {
    int sockfd;
    char buf[MAXLINE + 1];
    struct sockaddr_in servaddr;
    
    //создаем потоковый сокет
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        cout << "socket error";
    
    
    //заполняем структуру
    bzero(&servaddr, sizeof(servaddr)); // заполняем всю структуру нулями
    servaddr.sin_family = AF_INET; //будем работать с интернетом, а не с ...
    servaddr.sin_port = htons(13); /* сервер времени и даты */
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
    listen(sockfd, 8);
    
    time_t ticks;
    int client;
    
    while(1) {
        client = accept(sockfd, NULL, NULL);
        
        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "current time is %.24s\er\en", ctime(&ticks));
        write(client, buf, strlen(buf));
        
        close(client);
    }
    return(0);
}
