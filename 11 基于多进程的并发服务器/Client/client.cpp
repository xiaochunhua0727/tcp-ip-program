#include <iostream>
 #include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 30

int	main(int argc, char **argv){
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset( &serv_addr, 0, sizeof(serv_addr) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi("9190"));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        std::cout << "connect socket failed." << std::endl;
        exit(-1);
    }

    int read_cnt = 0;
    char buf[BUF_SIZE];
    
    // 等待服务器发送数据，收到EOF时会退出该循环
    while ((read_cnt = read(sock, buf, BUF_SIZE)) != 0){
        std::cout << "receive message: " << buf << std::endl;
    }
    
    close(sock);
    return 0;
}
