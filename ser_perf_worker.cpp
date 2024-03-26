#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {
    uint64_t buffer_size = -1;
    std::unique_ptr<char[]> buffer = IPC::receive_data(buffer_size);

    if(!buffer)
        return -1;
    
    torch::Tensor t;
    serialization::deseralize(t, std::move(buffer), buffer_size);

    std::cout << t << std::endl; 
    return 0;
}