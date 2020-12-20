#include <iostream>
#include <pthread.h>

int timeCnt = 0;
pthread_mutex_t mutex;

void* startRoutine(void*) {
    for (size_t i = 0; i < 10000; i++) {
        pthread_mutex_lock(&mutex);
        timeCnt = timeCnt + 1; 
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc,char** argv) {
    pthread_t shareSiteId[20];
    pthread_mutex_init(&mutex, NULL);
    for (size_t i = 0; i < 20; i++){
        pthread_create(shareSiteId + i, NULL,startRoutine, NULL);
    }
    
    for (size_t i = 0; i < 20; i++) {
        pthread_join(shareSiteId[i], NULL);
    }
    std::cout << "result: " << timeCnt << std::endl;
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
