#ifndef IPCHPP
#define IPCHPP

#include <memory>

#define FIFO1 "/tmp/my_fifo_1"
#define FIFO2 "/tmp/my_fifo_2"
#define FIFO3 "/tmp/my_fifo_3"
#define FIFO4 "/tmp/my_fifo_4"

namespace IPC {
    int create_ipc();
    bool send_data(int fifo, std::unique_ptr<char[]> buffer, uint64_t buffer_size);
    std::unique_ptr<char[]> receive_data(int fifo, uint64_t &buffer_size);
    void destroy_ipc(int fifo);
}

#endif // IPCHPP