#include <iostream>
 #include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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

    char message[30] = {0};
    unsigned long length = 0;
    read(sock,&length,sizeof(unsigned long));
    // 将网络字节序的数据转换成本地字节序
    length = ntohl(length);
    std::cout << "客户端需读取字符个数: " <<  length << std::endl;
    read(sock, message, length);
  
    std::cout << "message: " << message << std::endl;
    
    close(sock);
    return 0;
}
