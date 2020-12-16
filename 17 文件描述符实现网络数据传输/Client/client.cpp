#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
#include <stdio.h>

int main() { 
    int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET; 
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_addr.sin_port = htons(9190); 
    
    if (connect(client_sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1){
        std::cout << "connect socket failed." << std::endl;
        exit(1);
    }
    
    char buf[1024];
    FILE* readFp = fdopen(client_sockfd, "r");
    FILE* writeFp = fdopen(client_sockfd, "w");
    while (1){
        std::cout << "send message: ";
        fgets(buf, 1024, stdin);
        fputs(buf,writeFp);
        fflush(writeFp);
        fgets(buf, 1024, readFp);
        std::cout << "receive message: " << buf;
    }
    
    fclose(writeFp);
    fclose(readFp);
    close(client_sockfd); 
    return 0; 
}