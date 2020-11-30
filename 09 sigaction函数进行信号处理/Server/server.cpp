#include <iostream>
#include <signal.h>

void sigHandler(int signo) {
	std::cout << "正在对外部信号进行处理..." << std::endl;
	
	if (signo == SIGINT){
		std::cout << "外部的SIGINT信号处理完毕..." << std::endl;
	}
}

int main(int argc,char** argv) {
	struct sigaction act;
	act.sa_handler = sigHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, nullptr);

	while (1){
		
	}
	
	return 0;
}
