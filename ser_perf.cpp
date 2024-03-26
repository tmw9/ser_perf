#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    uint16_t dim = 1 + (rand() % 1024);

    torch::Tensor tensor = torch::rand({dim, dim});
    uint64_t buffer_size = 0;

    auto at =  serialization::serialize(tensor, buffer_size);

    IPC::send_data(std::move(at), buffer_size);
    return 0;
}