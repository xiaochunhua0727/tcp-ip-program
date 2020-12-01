#include <iostream>
#include <unistd.h>

#define  BUF_SIZE  30  

int	main(int argc, char **argv){
    int fds1[2], fds2[2];
    pipe(fds1);
    pipe(fds2);
    
    char str1[] = "who are you.";
    char str2[] = "Thank you for your message.";
    char buf[BUF_SIZE];
    pid_t pid = fork();
    if (pid == 0){
        write(fds1[1], str1,sizeof(str1));
        read(fds2[0], buf, BUF_SIZE);
        std::cout << "read message: " << buf << std::endl;
    }else {
        read(fds1[0], buf, BUF_SIZE);
        std::cout << "read message: " << buf << std::endl;
        write(fds2[1], str2, sizeof(str2));
        // 延时10秒，保证父进程退出时子进程已执行完毕
        sleep(10);
    }
    return 0;
}
