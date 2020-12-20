#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <list>

std::list<int> list1;
static sem_t writeSem;
static sem_t readSem;

void* WriteData(void* param) {
    for (size_t i = 0; i < 5; i++){
        sem_wait(&writeSem);
        std::cout << "write Data: " ;
        int data;
        std::cin >> data;
        list1.push_back(data);
        sem_post(&readSem);
    }
    return NULL;
} 

void* ReadData(void* param) {
    for (size_t i = 0; i < 5; i++){
        sem_wait(&readSem);
        int data = list1.front();
        list1.pop_front();
        std::cout << "read data: " << data << std::endl;
        sem_post(&writeSem);
    }
    return NULL;
}

int main(int argc,char** argv) {
    sem_init(&writeSem, 0, 1);
    sem_init(&readSem, 0, 0);

    pthread_t readID, writeId;
    pthread_create(&writeId, NULL, WriteData, NULL);
    pthread_create(&readID, NULL, ReadData, NULL);
    pthread_join(readID, NULL);
    pthread_join(writeId, NULL);

    sem_destroy(&writeSem);
    sem_destroy(&readSem);

    return 0;
}