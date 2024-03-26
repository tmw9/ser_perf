#include <torch/torch.h>

#include "serialization.hpp"

static size_t stream_size(std::stringstream &ss) {
    ss.seekg(0, std::ios::end);
    size_t size = ss.tellg();
    ss.seekg(0, std::ios::beg);

    return size;
}

std::unique_ptr<char[]> serialization::serialize(const torch::Tensor &t, uint64_t &buffer_size) {
    // I honestly don't like the below approach as it
    // converts the tensor to string stream and then I have to 
    // to go string stream make a copy of the contents
    // then copy those contents to a buffer
    std::stringstream ss;
    torch::save(t, ss);

    buffer_size = stream_size(ss) + sizeof(uint64_t);
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(buffer_size);

    memcpy(buffer.get(), ss.str().c_str(), buffer_size);
    
    return std::move(buffer);
}

void serialization::deseralize(torch::Tensor &t, std::unique_ptr<char[]> buffer, const uint64_t buffer_size) {
    std::stringstream ss;

    ss.write(buffer.get(), buffer_size);

    torch::load(t, ss);
}
