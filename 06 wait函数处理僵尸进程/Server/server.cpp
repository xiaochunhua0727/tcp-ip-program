#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

int	main(int argc, char **argv){
	int status;
	pid_t pid = fork();
	if (pid == 0){
		std::cout << "子进程处理流程..." << std::endl;
	}
	else {
		std::cout << "主进程执行代码..." << std::endl;
		wait(&status);  // 等待子进程执行完毕
		
		if (WIFEXITED(status)){
			std::cout << "子进程返回值: " << WEXITSTATUS(status) << std::endl;
		}
		
	}
	return 0;
}
