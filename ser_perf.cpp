#include <torch/torch.h>
#include <iostream>

int main() {
  srand(time(NULL));
  uint16_t dim = 1 + (rand() % 1024);

  torch::Tensor tensor = torch::rand({dim, dim});

  std::stringstream stream;
  torch::save(tensor, stream);

  torch::Tensor tensor2;
  torch::load(tensor2, stream);

  std::cout << tensor2 << std::endl;
}