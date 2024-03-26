#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"

int main() {
    srand(time(NULL));
    uint16_t dim = 1 + (rand() % 1024);

    torch::Tensor tensor = torch::rand({dim, dim});

    auto at =  serialize(tensor);
    torch::Tensor t;
    deseralize(t, at);
    std::cout << t << std::endl;
}