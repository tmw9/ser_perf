#ifndef IPCHPP
#define IPCHPP

#include <memory>

#define FIFO1 "/tmp/my_fifo_1"
#define FIFO2 "/tmp/my_fifo_2"
#define FIFO3 "/tmp/my_fifo_3"
#define FIFO4 "/tmp/my_fifo_4"
#define SOCKET "/tmp/perf.socket"

namespace ipc {


class IPC {
public:
    // virtual int create_ipc() = 0;
    // bool send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size);
    // std::unique_ptr<char[]> receive_data(uint64_t &buffer_size);

    bool _send_data(int ipc, std::unique_ptr<char[]> buffer, uint64_t buffer_size);
    std::unique_ptr<char[]> _receive_data(int ipc, uint64_t &buffer_size);

    virtual bool send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size) = 0;
    virtual std::unique_ptr<char[]> receive_data(uint64_t &buffer_size) = 0;

    virtual ~IPC() = default;

protected:
    IPC() = default;
};

class FIFO: public IPC {
private:
    std::unique_ptr<char> m_rd_path, m_wr_path;
    int m_rd_fifo, m_wr_fifo;
    
public:
    FIFO(char *, char *, int, int);
    ~FIFO();

    static std::unique_ptr<FIFO> create_ipc(const char *, const char *);
    bool send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size);
    std::unique_ptr<char[]> receive_data(uint64_t &buffer_size);
};

class SOCKT: public IPC {
private:
    std::unique_ptr<char> m_socket_path;
    bool m_server;

    int m_socket;

public:
    SOCKT(char *, int);
    ~SOCKT();

    static std::unique_ptr<SOCKT> create_ipc(const char *, const char *);
    bool send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size);
    std::unique_ptr<char[]> receive_data(uint64_t &buffer_size);
};

} // namespace ipc

#endif // IPCHPP