#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {
    auto ipc = ipc::IPCFactory::create_ipc(ipc::IPCFactory::IPCType::FIFO, FIFO2, FIFO1);
    // auto ipc = ipc::IPCFactory::create_ipc(ipc::IPCFactory::IPCType::SOCKT, SOCKET, "server");
    // auto ipc = ipc::IPCFactory::create_ipc(ipc::IPCFactory::IPCType::SHM, SHMFILE, "server");

    
    if(!ipc) 
        exit(1);

    // receive the buffer
    uint64_t buffer_size = -1;
    std::unique_ptr<char[]> buffer = ipc -> receive_data(buffer_size);

    // deseralize the tensor
    torch::Tensor t;
    serialization::deseralize(t, std::move(buffer), buffer_size);

    // perform the operation
    t = torch::square(t);

    // serailize the output
    buffer_size = -1;
    auto sq_buf = serialization::serialize(t, buffer_size);

    // send the data over ipc
    ipc -> send_data(std::move(sq_buf), buffer_size);

    return 0;

failed:
    return -1;
}