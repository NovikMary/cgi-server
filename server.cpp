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
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdlib.h>
#include <string>
#include <thread>

//using namespace std;
using std::thread;
using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::to_string;

void che_to_tam(int socket, char *type, char *addr, char *args) {
    int size = 20;
    char buf[size];
    char request[200];
    int k = 0;
    int fl = 0;
    int len;
    while (((len = recv(socket, &buf, size, 0)) > 0) && (fl == 0)) {
        for (int i = 0; i < len; i++) {
            if (buf[i] == '\n') {
                request[k] = buf[i];
                fl = 1;
                break;
            } else {
                request[k] = buf[i];
                k++;
            }
        }
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

void thread_function(const int &sock, const string &folder) {
    if(sock < 0)
    {
        cout << "accept" << endl;
        exit(0);
    }
    char type[5];
    char addres[100];
    char args[100];
    che_to_tam(sock, type, addres, args);
    if (strcmp(type, "GET") == 0) {
        cout << "GET request" << endl;

        if (strcmp(addres, "/") == 0)
            strcpy(addres, "/index.html");
        char fullname[1000];
        char name_for_exec[1002];
        cout << "Your requered adress is " << addres << endl;
        sprintf(fullname, "%s%s", folder.c_str(), addres);
        cout << addres << endl;

        sprintf(name_for_exec, "%s%s%s", "./", folder.c_str(), addres);
        struct stat file;
        
        if (stat(fullname, &file) != -1){
            if (access(fullname, F_OK | X_OK) == 0) {
                cout << "Executed file " << fullname << "\n";
                int p1[2];
                pipe(p1);
                if (!fork()){
                    close(p1[0]);
                    dup2(p1[1],1);
                    close(p1[1]);
                    execlp(name_for_exec, name_for_exec, NULL);
                    exit(0);
                } else {
                    close(p1[1]);

                    int part_size;
                    char buf[205];

                    string header_executed = "HTTP/1.1 200 OK\r\n";
                    send (sock, header_executed.c_str(), header_executed.length(), 0);

                    while ((part_size = read(p1[0], &buf, 200)) > 0) {
                        send(sock, buf, part_size, 0);
                        cout << buf;
                    }
                    cout << "\nfinal\n";
                    close(p1[0]);
                    wait(NULL);
                }
            } else if ((S_ISREG(file.st_mode) == true) && (access(fullname, F_OK | R_OK) == 0)) {
                cout << "Regular file file " << fullname << "\n";
                int fd = open(fullname, O_RDONLY);
                char *file_text;
                int length = lseek(fd, 0, SEEK_END);
                file_text = (char*)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
                string header_static = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + to_string(length) + "\r\n\r\n";

                send (sock, header_static.c_str(), header_static.length(), 0);
                send(sock, file_text, length, 0);
                close(fd);
            } else {
                cout << "Page not found - 1\n";
                string no_page = string(folder) + "/404.html";
                int fd = open(no_page.c_str(), O_RDONLY);
                char *file_text;
                int length = lseek(fd, 0, SEEK_END);
                string header_not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + to_string(length) + "\r\n\r\n";
                send (sock, header_not_found.c_str(), header_not_found.length(), 0);

                file_text = (char*)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
                send(sock, file_text, length, 0);
                close(fd);
            }
        } else {
            cout << "Page not found - 2\n";
            strcpy(fullname, "404.html");
            cout << fullname << endl;
            int fd = open(fullname, O_RDONLY);
            cout << fd << endl;
            char *file_text;
            int length = lseek(fd, 0, SEEK_END);

            string header_not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + to_string(length) + "\r\n\r\n";
            send (sock, header_not_found.c_str(), header_not_found.length(), 0);

            file_text = (char*)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
            send(sock, file_text, length, 0);
            close(fd);
        }
    }
    cout << "it's your time to die\n" << endl;
    close(sock);
}

void another() {//const int &sock, const string &folder) {
    cout << "HOLA" << endl;
}

int main(int argc, char *argv[]) {
    int sock, listener;
    struct sockaddr_in servaddr;
    char buf[1024];
    int bytes_read;
    string folder;

    cout << "starting server initialization" << endl;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        cout << "socket";
        return(0);
    }
    
    servaddr.sin_family = AF_INET;
    if (argc < 2) {
        servaddr.sin_port = htons(3425);
        cout << "server attached to port 3425" << endl;
    } else {
        servaddr.sin_port = htons(atoi(argv[1]));
        cout << "server attached to port " << argv[1] << endl;
    }
    if (argc < 3) {
       folder = "pages";
    }
    else {
        folder = argv[2];
    }
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        cout << "bind" << endl;
        return(0);
    }

    listen(listener, 8);

    cout << "server initialized on socket " << listener << endl;

    int counter = 0;

    while(1)
    {
        counter++;
        cout << "waiting for connection " << counter << endl;

        if ((sock = accept(listener, NULL, NULL)) > 0) {
            cout << "accept of connection " << counter << " is done" << endl;
            thread tr([&sock, &folder](){thread_function(sock, folder);});
            tr.detach();
        } else {
            cout << "accept <= 0\n";
        }
    }
    
    return 0;
}