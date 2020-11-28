#include <iostream>
#include <signal.h>

void sigHandler(int signo) {
	std::cout << "进入到信号处理函数内部..." << std::endl;
	if (signo == SIGINT){
		std::cout << "接收到SIGINT信号..." << std::endl;
	}
	
}

int	main(int argc, char **argv){
	signal(SIGINT,sigHandler);
	while (1)
	{
		/* code */
	}
	
	return 0;
}
