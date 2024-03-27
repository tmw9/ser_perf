#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // create ipc
    int ipc = IPC::create_ipc();
    if(ipc == -1)
        return -1;

    // create tensor
    uint16_t dim = 1 + (rand() % 10);
    torch::Tensor tensor = torch::rand({dim, dim});
    torch::Tensor sq_tensor;
    
    // serialize the tensor
    uint64_t buffer_size = 0;
    auto tensor_buffer =  serialization::serialize(tensor, buffer_size);

    // send data over ipc
    IPC::send_data(ipc, std::move(tensor_buffer), buffer_size);
    
    // receive data over ipc
    buffer_size = -1;
    auto sq_tensor_buffer = IPC::receive_data(ipc, buffer_size);
    if(!sq_tensor_buffer)
        goto failed;

    // deserailize the tensor
    serialization::deseralize(sq_tensor, std::move(sq_tensor_buffer), buffer_size);

    // check if the data is correct
    assert(torch::equal(torch::square(tensor), sq_tensor));

    // close the ipc
    IPC::destroy_ipc(ipc);

    return 0;

failed:
    IPC::destroy_ipc(ipc);
    return -1;
}