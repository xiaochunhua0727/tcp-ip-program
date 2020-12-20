#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

void* handle_client(void* arg) {
    int clientSock = *((int*) arg);
    char buf[1024];
    while (1){
        memset(buf, 0, 1024);
        int strLen = read(clientSock, buf, 1024);
        if (strLen == 0){
            break;
        }
        std::cout << "read message: " << buf;
        write(clientSock, buf, strLen);
    }
    std::cout << "close client:" << clientSock << std::endl;
    close(clientSock);
    return NULL;
} 

int main(int argc,char** argv) {
    int servSock = socket(AF_INET, SOCK_STREAM, 0);
    if (servSock == -1){
        std::cout << "create socket failed." << std::endl;
        exit(1);
    }
    
    struct sockaddr_in servAddr, clientAddr;
    socklen_t clientAddrlen = sizeof(clientAddr);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9190);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1){
        std::cout << "bind socket failed." << std::endl;
        exit(1);
    }
    
    if (listen(servSock, 5) == -1){
        std::cout << "listen socket failed." << std::endl;
        exit(1);
    }
    
    while (1){
        int clientSock = accept(servSock,(struct sockaddr*)&clientAddr, &clientAddrlen);
        pthread_t handleThreadID;
        pthread_create(&handleThreadID, NULL, handle_client, &clientSock);
        pthread_detach(handleThreadID);
        std::cout << "connect client: " << clientSock << std::endl;
    }
    
    close(servSock);
    return 0;
}