#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void sigHandler(int signo) {
	std::cout << "正在对外部信号进行处理..." << std::endl;
	int status;
	if (signo == SIGCHLD){
		waitpid(-1, &status, WNOHANG);
		std::cout << "对僵尸进程处理完毕" << std::endl;
	}
}

int main(int argc,char** argv) {
	struct sigaction act;
	act.sa_handler = sigHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	// 子进程终止时将触发SIGCHLD信号
	sigaction(SIGCHLD, &act, nullptr);

	pid_t pid = fork();
	if (pid == 0){
		std::cout << "子进程处理..." << std::endl;
	}else {
		// 主进程
		while (1){
			/* code */
		}
	}
	
	return 0;
}
