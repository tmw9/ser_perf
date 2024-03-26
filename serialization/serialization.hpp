#ifndef SERIALIZATION
#define SERIALIZATION

#include <torch/torch.h>

char *serialize(const torch::Tensor &t);

void deseralize(torch::Tensor &t, const char *buffer);

#endif // SERIALIZATION