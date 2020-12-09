#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>

int main() { 
    int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET; 
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_addr.sin_port = htons(8888); 
    
    if (connect(client_sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1){
        std::cout << "connect socket failed." << std::endl;
        exit(1);
    }
        
    char buf[30];
    while (1){
        std::cout << "send message: ";
        fgets(buf, 30, stdin);
        write(client_sockfd, buf, 30);
    }
       
    close(client_sockfd); 
    return 0; 
}