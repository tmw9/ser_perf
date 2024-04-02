#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {

    if(argc < 3) {
        std::cout << "Usage: ./ser_perf <IPCType> arg1 arg2" << std::endl;
        exit(1);
    }


    auto ipc = ipc::IPCFactory::create_ipc(argv[1], argv[2], argv[3]);
    
    if(!ipc)
        exit(1);

    while(true) {
        // receive the buffer
        uint64_t buffer_size = -1;
        std::unique_ptr<char[]> buffer = ipc -> receive_data(buffer_size);
        // deseralize the tensor
        torch::Tensor t = torch::zeros({1});
        serialization::InBuiltSerializer::deseralize(t, std::move(buffer), buffer_size);

        // perform the operation
        t = torch::square(t);

        // serailize the output
        buffer_size = -1;
        auto sq_buf = serialization::InBuiltSerializer::serialize(t, buffer_size);

        // send the data over ipc
        ipc -> send_data(std::move(sq_buf), buffer_size);

        std::cout << "SERVED CLIENT" << std::endl;
    }

    return 0;

failed:
    return -1;
}