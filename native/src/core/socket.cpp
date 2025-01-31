#include <fcntl.h>
#include <endian.h>

#include <socket.hpp>
#include <base.hpp>

using namespace std;

bool get_client_cred(int fd, sock_cred *cred) {
    socklen_t len = sizeof(ucred);
    if (getsockopt(fd, SOL_SOCKET, SO_PEERCRED, cred, &len) != 0)
        return false;
    char buf[4096];
    len = sizeof(buf);
    if (getsockopt(fd, SOL_SOCKET, SO_PEERSEC, buf, &len) != 0)
        len = 0;
    buf[len] = '\0';
    cred->context = buf;
    return true;
}

int read_int_be(int fd) {
    return ntohl(read_int(fd));
}

void write_int_be(int fd, int val) {
    write_int(fd, htonl(val));
}

bool read_string(int fd, std::string &str) {
    str.clear();
    auto len = read_any<size_t>(fd);
    str.resize(len);
    return xxread(fd, str.data(), len) == len;
}

string read_string(int fd) {
    string str;
    read_string(fd, str);
    return str;
}

void write_string(int fd, string_view str) {
    if (fd < 0) return;
    write_any(fd, str.size());
    xwrite(fd, str.data(), str.size());
}
