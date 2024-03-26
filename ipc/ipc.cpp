#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

#include "ipc.hpp"


static bool create_fifo() {
    if(mkfifo(FIFO1, S_IRUSR | S_IWUSR) && errno != EEXIST)
        return false;
    return true;
}

static int open_fifo() {
    int fifo = open(FIFO1, O_RDWR);
    return fifo;
}

bool IPC::send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size) {
    create_fifo();

    int fifo = open (FIFO1, O_RDWR);
    if(fifo == -1) {
        unlink(FIFO1);
        return false;
    }
    
    if(write(fifo, &buffer_size, sizeof(uint64_t)) != sizeof(uint64_t) ||
        write(fifo, buffer.get(), buffer_size) != buffer_size)
        return false;

    return true;
}

std::unique_ptr<char[]> IPC::receive_data(uint64_t &buffer_size) {
    buffer_size = 0;
    create_fifo();

    int fifo = open(FIFO1, O_RDWR);
    if(fifo == -1) {
        unlink(FIFO1);
        return nullptr;
    }

    if(read(fifo, &buffer_size, sizeof(uint64_t)) != sizeof(uint64_t))
        return nullptr;
    
    auto buffer = std::make_unique<char[]>(buffer_size);

    uint64_t data_read = 0;
    while((data_read += read(fifo, buffer.get() + data_read, buffer_size)) != buffer_size);

    return std::move(buffer);
}