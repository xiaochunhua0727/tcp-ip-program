#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/epoll.h>

#define EPOLL_SIZE 50

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
    
    int epfd = epoll_create(EPOLL_SIZE);
    struct epoll_event event;
    // 对servSock套接字监听，条件触发事件的发生
    event.data.fd = servSock;
    event.events = EPOLLIN;
    // 对事件进行注册
    epoll_ctl(epfd, EPOLL_CTL_ADD, servSock, &event);
    while (1){
        struct epoll_event* ep_events = (struct epoll_event*) malloc( sizeof(struct epoll_event) * EPOLL_SIZE );
        int event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if ( event_cnt == -1){
            std::cout << "epoll_wait() error..." << std::endl;
            break;
        }
        
        for (int cnt = 0; cnt < event_cnt; ++cnt) {
            if (servSock == ep_events[cnt].data.fd){
                int client_sock = accept(servSock, (struct sockaddr*)&clientAddr, &clientAddrlen);
                event.events = EPOLLIN;
                event.data.fd = client_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);
                std::cout << "connect client: " << client_sock << std::endl;
            }
            else {
                char buf[1024] = {0};
                int strlength = read(ep_events[cnt].data.fd, buf,1024);
                if (strlength == 0){
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[cnt].data.fd, NULL);
                    close(ep_events[cnt].data.fd);
                    std::cout << "close client: " << ep_events[cnt].data.fd << std::endl;
                }
                else {
                    write(ep_events[cnt].data.fd, buf, strlength);
                }
            }

        }
    }
    
    close(servSock);
    close(epfd);
    return 0;
}