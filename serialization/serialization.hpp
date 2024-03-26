#ifndef SERIALIZATION
#define SERIALIZATION

#include <torch/torch.h>

namespace serialization {
    std::unique_ptr<char[]> serialize(const torch::Tensor &t);

    void deseralize(torch::Tensor &t, std::unique_ptr<char[]> buffer);
}
#endif // SERIALIZATION