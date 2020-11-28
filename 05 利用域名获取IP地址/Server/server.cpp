#include <netdb.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

int	main(int argc, char **argv){
	struct hostent* host = gethostbyname("www.baidu.com");
	for (size_t i = 0; host->h_addr_list[i]; i++){
		std::cout << "IP: " << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << std::endl;
	}
	
	return 0;
}