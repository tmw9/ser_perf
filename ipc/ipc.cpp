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

int IPC::create_ipc() {
    if(!create_fifo())
        return false;

    int fifo = open (FIFO1, O_RDWR);
    if(fifo == -1) {
        unlink(FIFO1);
        return -1;
    }
    
    return fifo;
}


bool IPC::send_data(int fifo, std::unique_ptr<char[]> buffer, uint64_t buffer_size) {
    std::cout << "SENDING DATA BYTES: " << buffer_size << std::endl;
    if(write(fifo, &buffer_size, sizeof(uint64_t)) != sizeof(uint64_t) ||
        write(fifo, buffer.get(), buffer_size) != buffer_size) 
        return false;

    std::cout << "DATA SEND COMPLETE" << std::endl;
    return true;
}

std::unique_ptr<char[]> IPC::receive_data(int fifo, uint64_t &buffer_size) {
    std::cout << "RECEIVING DATA" << std::endl;
    buffer_size = 0;
    if(read(fifo, &buffer_size, sizeof(uint64_t)) != sizeof(uint64_t))
        return nullptr;

    std::cout << "RECEIVING DATA BYTES: " << buffer_size << std::endl;
    auto buffer = std::make_unique<char[]>(buffer_size);

    uint64_t data_read = 0;
    while((data_read += read(fifo, buffer.get() + data_read, buffer_size)) != buffer_size) {
        std::cout << "RECEIVED SOME DATA: " << data_read << std::endl;
    }

    std::cout << "RECEIVING DATA COMPLETE" << std::endl;

    return std::move(buffer);
}

void IPC::destroy_ipc(int fifo) {
    close(fifo);
    unlink(FIFO1);
}
