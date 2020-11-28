#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

int	main(int argc, char **argv){
	int status;
	pid_t pid = fork();
	if (pid == 0){
		std::cout << "子进程处理流程..." << std::endl;
		sleep(30);
	}
	else {
		std::cout << "主进程执行代码..." << std::endl;
		// 循环执行，直到有子进程结束
		while (!waitpid(-1, &status, WNOHANG)) {
			sleep(3);
			std::cout << "等待子进程结束." << std::endl;
		}
		
		if (WIFEXITED(status)){
			std::cout << "子进程返回值: " << WEXITSTATUS(status) << std::endl;
		}
		
	}
	return 0;
}
