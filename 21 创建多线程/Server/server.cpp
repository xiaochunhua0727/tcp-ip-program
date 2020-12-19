#include <iostream>
#include <unistd.h>
#include <pthread.h>

void* startRoutine(void* params) {
    for (int cnt = 0; cnt < 20; ++cnt){
        std::cout << "runing thread..." << std::endl;
        sleep(2);
    }
    return NULL;
}

int	main(int argc, char **argv){
    // 线程无返回值
    void* ret;   
    pthread_t shareID;
    
    if (pthread_create(&shareID, NULL, &startRoutine, NULL) != 0){
        std::cout << "create thread failed." << std::endl;
        return -1;
    }
    
    if (pthread_join(shareID,&ret) != 0){
        std::cout << "thread join failed." << std::endl;
        return -1;
    }
    
    return 0;
}
