#include <torch/torch.h>

#include "serialization.hpp"

size_t stream_size(std::stringstream &ss) {
    ss.seekg(0, std::ios::end);
    size_t size = ss.tellg();
    ss.seekg(0, std::ios::beg);

    return size;
}

char *serialize(const torch::Tensor &t) {
    // I honestly don't like the below approach as it
    // converts the tensor to string stream and then I have to 
    // to go string stream make a copy of the contents
    // then copy those contents to a buffer
    std::stringstream ss;
    torch::save(t, ss);

    size_t ss_size = stream_size(ss);
    char *buffer = new char[ss_size + sizeof(uint64_t)];

    memcpy(buffer, &ss_size, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t), ss.str().c_str(), ss_size);
    
    return std::move(buffer);
}

void deseralize(torch::Tensor &t, const char *buffer) {
    std::stringstream ss;

    size_t ss_size;
    memcpy(&ss_size, buffer, sizeof(uint64_t));

    ss.write(buffer + sizeof(uint64_t), ss_size);

    torch::load(t, ss);
    // return std::move(t);
}