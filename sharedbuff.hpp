#ifndef SHARED_BUFFER
#define SHARED_BUFFER

#include <iostream>
#include <semaphore.h>
#include <thread>
#include <ctime>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>

const int BUFFER_SIZE = 2;
struct SharedData {
    sem_t empty, full, mutex;
    int buffer[BUFFER_SIZE];
};

#endif
