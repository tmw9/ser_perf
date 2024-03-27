#include <torch/torch.h>
#include <iostream>

#include "serialization/serialization.hpp"
#include "ipc/ipc.hpp"

int main(int argc, char *argv[]) {
    // create ipc
    // int ipc = ipc::create_ipc();
    // if(ipc == -1)
    //     return -1;
    // int ipc = ipc::create_socket_server(SOCKET);
    // std::unique_ptr<ipc::IPC> ipc = ipc::FIFO::create_ipc(FIFO2, FIFO1);
    std::unique_ptr<ipc::IPC> ipc = ipc::SOCKT::create_ipc(SOCKET, "server");
    if(!ipc) {
        std::cout << "IT DIDN'T WORK OUT" << std::endl;
        exit(1);
    }

    // receive the buffer
    uint64_t buffer_size = -1;
    std::unique_ptr<char[]> buffer = ipc -> receive_data(buffer_size);

    // deseralize the tensor
    torch::Tensor t;
    serialization::deseralize(t, std::move(buffer), buffer_size);

    // perform the operation
    t = torch::square(t);

    // serailize the output
    buffer_size = -1;
    auto sq_buf = serialization::serialize(t, buffer_size);

    // send the data over ipc
    ipc -> send_data(std::move(sq_buf), buffer_size);

    // // close the ipc
    // ipc::destroy_ipc(ipc);

    return 0;

failed:
    // ipc::destroy_ipc(ipc);
    return -1;
}