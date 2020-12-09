#include <iostream>
#include <sys/uio.h>

int main(int argc,char** argv) {
    struct iovec vec[2];
    char buf1[30] = {0};
    char buf2[30] = {0};
    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 30;
    readv(0, vec, 2);
    std::cout << "buf1: " << buf1 << std::endl;
    std::cout << "buf2: " << buf2;
    writev(1, vec, 2);
    return 0;
}