#ifndef IPCHPP
#define IPCHPP

#include <memory>
#include <semaphore.h>

#define SHM_BUF_SIZE    (1024 * 1024 * 512UL)

namespace ipc {


class IPC {
public:
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

    int m_server_socket, m_client_socket;

public:
    SOCKT(char *, bool, int, int);
    ~SOCKT();

    static std::unique_ptr<SOCKT> create_ipc(const char *, const char *);
    bool send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size);
    std::unique_ptr<char[]> receive_data(uint64_t &buffer_size);
};

class SHM: public IPC {
private:
    std::unique_ptr<char> m_shm_path;
    bool m_server;

    struct shmbuf {
        sem_t  server;            /* POSIX unnamed semaphore */
        sem_t  client;            /* POSIX unnamed semaphore */
        size_t bytes_written;             /* Number of bytes used in 'buf' */
        char   buffer[SHM_BUF_SIZE];   /* Data being transferred */
    };
    shmbuf *m_shmbuf;

public:
    SHM(char *, shmbuf *, bool);
    ~SHM();

    static std::unique_ptr<SHM> create_ipc(const char *, const char *);
    bool send_data(std::unique_ptr<char[]> buffer, uint64_t buffer_size);
    std::unique_ptr<char[]> receive_data(uint64_t &buffer_size);
};

class IPCFactory {
public:
    static std::unique_ptr<IPC> create_ipc(const char *ipc_type_cstr, const char *arg1, const char *arg2) {
        std::string ipctype(ipc_type_cstr);
        if(ipctype == "FIFO")
            return FIFO::create_ipc(arg1, arg2);
        else if(ipctype == "SOCKET")
            return SOCKT::create_ipc(arg1, arg2);
        else if(ipctype == "SHM")
            return SHM::create_ipc(arg1, arg2);
        else
            return nullptr;
    }
};

} // namespace ipc

#endif // IPCHPP