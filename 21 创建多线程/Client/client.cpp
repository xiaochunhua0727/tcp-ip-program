#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int sock;
	char buf[BUF_SIZE];
	char recvBuf[BUF_SIZE];
	struct sockaddr_in serv_addr;
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(9190);
  
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	while(1){
		memset(buf, 0, 1024);
		memset(recvBuf, 0, 1024);
		std::cout << "send message: ";
		fgets(buf, 1024, stdin);
		write(sock, buf, 1024);
		read(sock, recvBuf, 1024);
		std::cout << "read buf: " << recvBuf;
	 }  

	close(sock);
	return 0;
}