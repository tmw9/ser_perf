#include <torch/torch.h>

#include "serialization.hpp"


static size_t stream_size(std::stringstream &ss) {
    ss.seekg(0, std::ios::end);
    size_t size = ss.tellg();
    ss.seekg(0, std::ios::beg);

    return size;
}

std::unique_ptr<char[]> serialization::InBuiltSerializer::serialize_impl(const torch::Tensor &t, uint64_t &buffer_size) {
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

void serialization::InBuiltSerializer::deseralize(torch::Tensor &t, std::unique_ptr<char[]> buffer, const uint64_t buffer_size) {
    std::stringstream ss;
    ss.write(buffer.get(), buffer_size);
    torch::load(t, ss);
}

std::unique_ptr<char[]> serialization::RawPtrSerializer::serialize_impl(const torch::Tensor &t, uint64_t &buffer_size) {
    int64_t dims = t.dim();
    int64_t total_elements = 1;
    for(int64_t i = 0; i < dims; ++i)
        total_elements *= t.sizes()[i];
    
    buffer_size = total_elements * sizeof(_Float32) + dims * sizeof(int64_t) + sizeof(int64_t);
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(buffer_size);

    int64_t start_write = 0;
    memcpy(buffer.get() + start_write, &dims, sizeof(int64_t));
    start_write += sizeof(int64_t);

    for(int64_t i = 0; i < dims; ++i) {
        int64_t dim = t.sizes()[i];
        memcpy(buffer.get() + start_write, &dim, sizeof(int64_t));
        start_write += sizeof(int64_t);
    }
    memcpy(buffer.get() + start_write, t.data_ptr<float>(), total_elements * sizeof(_Float32));

    return std::move(buffer);
}


void serialization::RawPtrSerializer::deseralize(torch::Tensor &t, std::unique_ptr<char[]> buffer, const uint64_t buffer_size) {
    int64_t num_dims, start_read = 0;
    memcpy(&num_dims, buffer.get() + start_read, sizeof(int64_t));
    start_read += sizeof(int64_t);
    
    std::vector<int64_t> dimensions;
    int64_t total_elements = 1;
    for(int64_t i = 0; i < num_dims; ++i) {
        int64_t dim;
        memcpy(&dim, buffer.get() + start_read, sizeof(int64_t));
        start_read += sizeof(int64_t);

        dimensions.push_back(dim);
        total_elements *= dim;
    }
    auto a = at::IntArrayRef(dimensions);

    t.resize_(dimensions);
    memcpy(t.data_ptr<float>(), buffer.get() + start_read, total_elements * sizeof(float));
}
