#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    auto ipc = ipc::IPCFactory::create_ipc(ipc::IPCFactory::IPCType::FIFO, FIFO1, FIFO2);
    // auto ipc = ipc::IPCFactory::create_ipc(ipc::IPCFactory::IPCType::SOCKT, SOCKET, "client");
    // auto ipc = ipc::IPCFactory::create_ipc(ipc::IPCFactory::IPCType::SHM, SHMFILE, "client");

    // create tensor
    uint16_t dim = 1 + (rand() % 1024);
    torch::Tensor tensor = torch::rand({dim, dim});
    torch::Tensor sq_tensor;
    
    // serialize the tensor
    uint64_t buffer_size = 0;
    auto tensor_buffer =  serialization::serialize(tensor, buffer_size);

    // send data over ipc
    ipc -> send_data(std::move(tensor_buffer), buffer_size);
    
    // receive data over ipc
    buffer_size = -1;
    auto sq_tensor_buffer = ipc -> receive_data(buffer_size);
    if(!sq_tensor_buffer)
        goto failed;

    // deserailize the tensor
    serialization::deseralize(sq_tensor, std::move(sq_tensor_buffer), buffer_size);

    // check if the data is correct
    assert(torch::equal(torch::square(tensor), sq_tensor));

    return 0;

failed:
    return -1;


}