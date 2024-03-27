#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {
    // create ipc
    int ipc = IPC::create_ipc();
    if(ipc == -1)
        return -1;

    // receive the buffer
    uint64_t buffer_size = -1;
    std::unique_ptr<char[]> buffer = IPC::receive_data(ipc, buffer_size);

    // deseralize the tensor
    torch::Tensor t;
    serialization::deseralize(t, std::move(buffer), buffer_size);

    // perform the operation
    t = torch::square(t);

    // serailize the output
    buffer_size = -1;
    auto sq_buf = serialization::serialize(t, buffer_size);

    // send the data over ipc
    IPC::send_data(ipc, std::move(sq_buf), buffer_size);

    // close the ipc
    IPC::destroy_ipc(ipc);

    return 0;

failed:
    IPC::destroy_ipc(ipc);
    return -1;
}