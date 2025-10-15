#include "sharedbuff.hpp"

// producer runs on infinite loop like the consumer
void producer(SharedData* data) {
    while (true) {
        sem_wait(&data->empty);
        sem_wait(&data->mutex);

        int item_produced = 0;
        for (int i = 0; i < BUFFER_SIZE; i++) {
            data->buffer[i] = rand() % 100 + 1;
            item_produced += data->buffer[i];
        }

        std::cout << "[*] Produced: " << item_produced << std::endl;

        sem_post(&data->mutex);
        sem_post(&data->full);  // signal when buffer has been filled by producer

        sleep(1);
    }
}

int main() {
    srand(time(0));

    // attempt creating shared memory buffer
    int shm_fd = shm_open("/shared_buffer", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Error: Failed to open shared memory." << std::endl;
        return 1;
    }

    // modify the size of the buffer
    ftruncate(shm_fd, sizeof(SharedData));
    if (shm_fd == -1) {
        std::cerr << "[-] Error: Failed to open (shm)shared_buffer." << std::endl;
        return 1;
    }

    // attempt mapping
    SharedData* data = (SharedData*) mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        std::cerr << "[-] Error: Failed to map shared memory." << std::endl;
        return 1;
    }

    // semaphores
    sem_init(&data->empty, 1, BUFFER_SIZE); // buffer is empty
    sem_init(&data->full, 1, 0);            // start of buffer is empty
    sem_init(&data->mutex, 1, 1);           // mutex is used for mutual-exclusion

    std::thread producer_thread(producer, data);
    producer_thread.join();

    return 0;
}
