#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <sys/ioctl.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <iostream>
#include <string.h>

int main() { 
    int server_sockfd, client_sockfd; 
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr, client_addr; 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_addr.sin_port = htons(8888); 
    bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sockfd, 5); 

	socklen_t client_len; 
    fd_set readfds, cpyfds;
    FD_ZERO(&readfds); 
    FD_SET(server_sockfd, &readfds);

    while(1) {
        cpyfds = readfds;

        int fdsNum = select(FD_SETSIZE, &cpyfds, (fd_set *)0, (fd_set *)0, (struct timeval *) 0); 
        if(fdsNum < 1) { 
            std::cout << "select failed." << std::endl;
            exit(1); 
        } 

        for(int fd = 0; fd < FD_SETSIZE; fd++) {
            if(FD_ISSET(fd, &cpyfds)) { 
                if(fd == server_sockfd) { 
                    client_len = sizeof(client_addr); 
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len); 
                    FD_SET(client_sockfd, &readfds);
                    std::cout << "new client: " << client_sockfd << std::endl;
                } 
                else { 
                    int nread; 
                    ioctl(fd, FIONREAD, &nread);
                    
                    /*客户数据请求完毕，关闭套接字，从集合中清除相应描述符 */
                    if(nread == 0) { 
                        close(fd); 
                        FD_CLR(fd, &readfds); 
                        std::cout << "remove client: " << fd << std::endl;
                    } 
                    else { 
                        char message[30]; 
                        while (1) {
                            // 检查缓冲区中是否存在数据
                            int strLen = recv(fd, message, 29, MSG_PEEK | MSG_DONTWAIT);
                            if ( strLen > 0 ){
                                break;
                            }
                        }
                        
                        memset(message, 0, sizeof(message));
                        recv(fd, message, 30, 0); 
						std::cout << "receive message: " << message;
                    } 
                } 
            } 
        } 
    } 

    return 0;
}