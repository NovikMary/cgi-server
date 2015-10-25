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

using namespace std;

int main()
{
    int sock, listener;
    struct sockaddr_in servaddr;
    char buf[1024];
    int bytes_read;

    cout << "starting server initialization" << endl;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        cout << "socket";
        return(0);
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(3425);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        cout << "bind" << endl;
        return(0);
    }

    listen(listener, 8);

    cout << "server initialized on socket " << listener << endl;

    int counter = 0;
    cout << "Keep working? (y/n)" << endl;
    while(1)
    {
        char t;
        cin >> t;
        if (t != 'y')
            break;
        counter++;
        cout << "waiting for connection " << counter << endl;
        sock = accept(listener, NULL, NULL);
        cout << "accept done" << endl;
        if(sock < 0)
        {
            cout << "accept" << endl;
            return(0);
        }

        while(1)
        {
            bytes_read = recv(sock, buf, 1024, 0);
            //for (int i = 0; i < bytes_read; i++) {
              //  *(buf + 8 + i) = *(buf + i);
            //}
            strcpy(buf + 8, buf);
            strcpy(buf, "Hello, ");
            if(bytes_read <= 0) {
                cout << "connection " << counter << " finished. Continue? (y/n)" << endl;
                close(sock);
                break;
            }
            send(sock, buf, bytes_read + 8, 0);
        }
    
        close(sock);
    }
    
    return 0;
}