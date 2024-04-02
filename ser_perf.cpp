#include <torch/torch.h>
#include <iostream>
#include <chrono>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if(argc < 4) {
        std::cout << "Usage: ./ser_perf <IPCType> arg1 arg2 <dimensions>" << std::endl;
        exit(1);
    }
    
    uint16_t dim = std::stoi(argv[4]);
    auto ipc = ipc::IPCFactory::create_ipc(argv[1], argv[2], argv[3]);
    if(!ipc) {
        perror("WHAT HAPPENED: ");
        return 1;
    }

    std::chrono::high_resolution_clock::time_point t1, t2;

    // create tensor
    torch::Tensor tensor = torch::rand({dim, dim});
    torch::Tensor sq_tensor = torch::zeros({1});
    
    t1 = std::chrono::high_resolution_clock::now();

    // serialize the tensor
    uint64_t buffer_size = 0;
    auto tensor_buffer =  serialization::InBuiltSerializer::serialize(tensor, buffer_size);

    // send data over ipc
    ipc -> send_data(std::move(tensor_buffer), buffer_size);
    
    // receive data over ipc
    buffer_size = -1;
    auto sq_tensor_buffer = ipc -> receive_data(buffer_size);

    // deserailize the tensor
    serialization::InBuiltSerializer::deseralize(sq_tensor, std::move(sq_tensor_buffer), buffer_size);

    t2 = std::chrono::high_resolution_clock::now();

    // check if the data is correct
    assert(torch::equal(torch::square(tensor), sq_tensor));

    auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
    std::cout << argv[1] << " " << dim << " " << time_span.count() << std::endl;
    
    return 0;

failed:
    return -1;
}