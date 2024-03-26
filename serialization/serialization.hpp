#ifndef SERIALIZATION
#define SERIALIZATION

#include <torch/torch.h>

namespace serialization {
    std::unique_ptr<char[]> serialize(const torch::Tensor &t, uint64_t &buffer_size);

    void deseralize(torch::Tensor &t, std::unique_ptr<char[]> buffer, const uint64_t buffer_size);
}
#endif // SERIALIZATION