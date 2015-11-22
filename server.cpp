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

#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdlib.h>
#include <string>

using namespace std;

void che_to_tam(int socket, char *type, char *addr, char *args) {
    int size = 20;
    char buf[size];
    char request[200];
    int k = 0;
    int fl = 0;
    int len;
    while (((len = recv(socket, &buf, size, 0)) > 0) && (fl == 0)) {
        for (int i = 0; i < len; i++) {
            if (buf[i] == '\n') {//&& (k != 0) && (request[k - 1] == '\r')){
                request[k] = buf[i];
                fl = 1;
                break;
            } else {
                request[k] = buf[i];
                k++;
            }
        }
        cout << request << endl;
    }
    int i = 0;
    while (request[i] != ' ') {
        type[i] = request[i];
        i++;
    }
    type[i] = '\0';
    i++;
    int j = 0;
    while (request[i + j] != '?' && request[i + j] != ' ') {
        addr[j] = request[i + j];
        j++;
    }
    addr[j] = '\0';
    i = i + j + 1;
    j = 0;
    if (request[i - 1] == '?') {
        while (request[i + j] != ' ') {
            args[j] = request[i + j];
            j++;
        }
    }
    args[j] = '\0';
}


int main(int argc, char *argv[]){
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

    while(1)
    {
        /*char t;
        cin >> t;
        if (t != 'y')
            break;*/
        counter++;
        cout << "waiting for connection " << counter << endl;



        if ((sock = accept(listener, NULL, NULL)) > 0) { //--- дескриптор файла, который необходимо обработать и считать все нужное из файла до \r\n\r\n
            if (!fork()) {
                cout << "accept of connection " << counter << " is done" << endl;
                if(sock < 0)
                {
                    cout << "accept" << endl;
                    return(0);
                }

                while(1)
                {
                    char type[5];
                    char addres[100];
                    char args[100];
                    che_to_tam(sock, type, addres, args);
                    if (strcmp(type, "GET") == 0) {
                        char fullname[1000];
                        sprintf(fullname, "%s/%s", argv[1], addres);
                        struct stat file;
                        if (stat(fullname, &file) != -1){
                            if ((S_ISREG(file.st_mode) == true) && (access(fullname, F_OK) == 0) && ((access(fullname, R_OK) == 0))) {

                                int fd = open(fullname, O_RDONLY);
                                char *file_text; 
                                int length = lseek(fd, 0, SEEK_END);
                                file_text = (char*)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);

                                send(sock, file_text, length, 0);
                                close(fd);
                            } else if ((access(fullname, F_OK) == 0) &&(access(fullname, X_OK) == 0)){
                                int p1[2];
                                pipe(p1);
                                if (!fork()){
                                    close(p1[0]);
                                    dup2(p1[1],1);
                                    close(p1[1]);
                                    execlp(addres, addres, args);
                                    exit(0);
                                } else {
                                    close(p1[1]);
                                    wait(NULL);
                                    int part_size;
                                    char buf[200];
                                    while ((part_size = read(p1[0], &buf, 200)) > 0) {
                                        send(sock, buf,part_size,0);
                                    }
                                }
                            } else {
                            send(sock, "file does not exist\n", 20, 0);
                            }
                        }
                    }
                }
            
                close(sock);
                exit(0);
            }
        }
    }
    
    return 0;
}