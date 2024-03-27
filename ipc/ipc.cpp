#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "ipc.hpp"

namespace ipc {

bool IPC::_send_data(int fifo, std::unique_ptr<char[]> buffer, uint64_t buffer_size) {
    std::cout << "SENDING DATA BYTES: " << buffer_size << std::endl;
    if(write(fifo, &buffer_size, sizeof(uint64_t)) != sizeof(uint64_t) ||
        write(fifo, buffer.get(), buffer_size) != buffer_size) 
        return false;

    std::cout << "DATA SEND COMPLETE" << std::endl;
    return true;
}

std::unique_ptr<char[]> IPC::_receive_data(int fifo, uint64_t &buffer_size) {
    std::cout << "RECEIVING DATA" << std::endl;
    buffer_size = 0;
    if(read(fifo, &buffer_size, sizeof(uint64_t)) != sizeof(uint64_t))
        return nullptr;

    std::cout << "RECEIVING DATA BYTES: " << buffer_size << std::endl;
    auto buffer = std::make_unique<char[]>(buffer_size);

    uint64_t data_read = 0;
    while((data_read += read(fifo, buffer.get() + data_read, buffer_size)) != buffer_size) {
        std::cout << "RECEIVED SOME DATA: " << data_read << std::endl;
    }

    std::cout << "RECEIVING DATA COMPLETE" << std::endl;

    return std::move(buffer);
}

FIFO::FIFO(char *rd_fifo_path, char *wr_fifo_path, int rd_fifo, int wr_fifo) : 
                                                m_rd_fifo(rd_fifo), m_wr_fifo(wr_fifo) {
    m_rd_path = std::unique_ptr<char>(rd_fifo_path);
    m_wr_path = std::unique_ptr<char>(wr_fifo_path);
}


std::unique_ptr<FIFO> FIFO::create_ipc(const char *rd_fifo_path, const char *wr_fifo_path) {
    char *rd_fifo_pth = new char[strlen(rd_fifo_path)];
    char *wr_fifo_pth = new char[strlen(wr_fifo_path)];
    int rd_fifo = -1, wr_fifo = -1;
    
    strcpy(rd_fifo_pth, rd_fifo_path);
    strcpy(wr_fifo_pth, wr_fifo_path);

    if((mkfifo(rd_fifo_pth, S_IRUSR | S_IWUSR) != 0 && errno != EEXIST) ||
        (mkfifo(wr_fifo_pth, S_IRUSR | S_IWUSR) != 0 && errno != EEXIST))
        goto cleanup;
    
    rd_fifo = open(rd_fifo_path, O_RDWR);
    wr_fifo = open(wr_fifo_path, O_RDWR);

    return std::move(std::make_unique<FIFO>(rd_fifo_pth, wr_fifo_pth, rd_fifo, wr_fifo));

cleanup:
    unlink(rd_fifo_path);
    unlink(wr_fifo_path);
    free(rd_fifo_pth);
    free(wr_fifo_pth);

    return nullptr;
}

bool FIFO::send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size) {
    std::cout << "IN SEND DATA METHOD" << std::endl;
    return _send_data(m_wr_fifo, std::move(buffer), buffer_size);
}

std::unique_ptr<char[]> FIFO::receive_data(uint64_t &buffer_size) {
    return _receive_data(m_rd_fifo, buffer_size);
}

FIFO::~FIFO() {
    close(m_rd_fifo);
    close(m_wr_fifo);

    unlink(m_rd_path.get());
    unlink(m_wr_path.get());
}

SOCKT::SOCKT(char *socket_path, int socket) : m_socket(socket) {
    m_socket_path = std::unique_ptr<char>(socket_path);
}

std::unique_ptr<SOCKT> SOCKT::create_ipc(const char *_socket_path, const char *host) {
    char *socket_path = new char[strlen(_socket_path)];
    sockaddr_un socket_addr, client_addr;
    int socket_fd = -1, len = -1, client_socket = -1;

    strcpy(socket_path, _socket_path);
        
    // create a socket
    if((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        goto onerror;
        
    // set the socket address structure to point to socket file
    memset(&socket_addr, 0, sizeof(socket_addr));
    socket_addr.sun_family = AF_UNIX;
    strncpy(socket_addr.sun_path, socket_path, sizeof(socket_addr.sun_path) - 1);

    if(!strcmp(host, "server")) {
        // bind socket to socket file
        len = sizeof(socket_addr.sun_family) + strlen(socket_addr.sun_path);
        if(bind(socket_fd, (sockaddr *)(&socket_addr), len) < 0) {
            perror("BIND: ");
            goto onerror;
        }
        
        // listen on the socket
        if(listen(socket_fd, 1000) < 0)
            goto onerror;

        if((client_socket = accept(socket_fd, nullptr, nullptr)) < 0)
            goto onerror;

        close(socket_fd);
        return std::move(std::make_unique<SOCKT>(socket_path, client_socket));
    } else if(!strcmp(host, "client")) {
        if(connect(socket_fd, (sockaddr *) &socket_addr, sizeof(socket_addr)) < 0)
            goto onerror;
        
        return std::move(std::make_unique<SOCKT>(socket_path, socket_fd));
    }

onerror:
    unlink(socket_path);
    free(socket_path);
    close(socket_fd);
    return nullptr;   
}

SOCKT::~SOCKT() {
    close(m_socket);
    unlink(m_socket_path.get());
}

bool SOCKT::send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size) {
    return _send_data(m_socket, std::move(buffer), buffer_size);
}

std::unique_ptr<char[]> SOCKT::receive_data(uint64_t &buffer_size) {
    return _receive_data(m_socket, buffer_size);
}

} // namespace ipc