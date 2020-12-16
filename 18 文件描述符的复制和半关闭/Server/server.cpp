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
    
    char buf[1024] = {0};
    FILE* readFp = fdopen(clientSock, "r");
    // 利用dup函数进行文件描述符的复制
    FILE* writeFp = fdopen(dup(clientSock), "w");
    
    fputs("hello world.\n", writeFp);
    fputs("i love c++. \n", writeFp);
    fflush(writeFp);
    shutdown( fileno(writeFp), SHUT_WR );
    fclose(writeFp);
    fgets(buf, 1024, readFp);
    std::cout << "receive buf: " << buf << std::endl;
        
    fclose(readFp);
    close(servSock);
    return 0;
}