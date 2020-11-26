#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>

#define  BUF_SIZE 100

int main(int argc,char** argv) {
    // serv_sock套接字用于接收客户端的套接字请求连接
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if ( serv_sock == -1){
        std::cout << "create socket failed." << std::endl;
        exit(-1);
    }
    
    struct sockaddr_in serv_addr;
    memset( &serv_addr, 0, sizeof(serv_addr) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi("9190"));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(serv_sock, (struct sockaddr*)&serv_addr,sizeof(struct sockaddr)) == -1) {
        std::cout << "bind socket failed." << std::endl;
        exit(-1);
    }
    
    if ( listen(serv_sock,5) == -1 ){
        std::cout << "listen socket failed." << std::endl;
        exit(-1);
    }
    
    // 与服务端连接的客户端信息保存在client_addr中
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in) );
    socklen_t clientAddrLength = sizeof(client_addr);
    // clientSock套接字用于同客户端套接字进行通信
    int clientSock = accept(serv_sock, (struct sockaddr*)&client_addr, &clientAddrLength);
    if (clientSock == -1){
        std::cout << "accept socket failed." << std::endl;
        exit(-1);
    }

    char buf[BUF_SIZE];
    int read_cnt = 0; 
    FILE* fp = fopen("readme.txt", "rb");
    while (1){
        read_cnt = fread(buf, 1, BUF_SIZE, fp);
        // 读取的个数小于指定个数，表明文件已经读取到末尾
        if (read_cnt < BUF_SIZE){
            write(clientSock,buf,read_cnt);
            break;
        }
        write(clientSock, buf, BUF_SIZE);
    }
    
    // 关闭输出流，此时会向客户端发送一个EOF标记
    // shutdown(clientSock, SHUT_WR);
    read(clientSock, buf, BUF_SIZE);
    std::cout << "message: " << buf << std::endl;

    fclose(fp);
    close(clientSock);
    close(serv_sock);

    return 0;
}