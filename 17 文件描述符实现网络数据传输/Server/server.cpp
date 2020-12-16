#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

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
    
    int clientSock = accept(servSock, (struct sockaddr*) &clientAddr, &clientAddrlen);
    if (clientSock == -1){
        std::cout << "accept socket failed." << std::endl;
        exit(1);
    }
    
    FILE* readFp = fdopen(clientSock, "r");
    FILE* writeFp = fdopen(clientSock, "w");
    char buf[30] = {0};
    // 将从客户端读取到的数据发送给客户端
    while ( !feof(readFp) ){
        memset(buf, 0 , 30);
        fgets(buf, 30, readFp);
        fputs(buf, writeFp);
        fflush(writeFp);
    }
    
    fclose(writeFp);
    fclose(readFp);
    close(servSock);
    return 0;
}