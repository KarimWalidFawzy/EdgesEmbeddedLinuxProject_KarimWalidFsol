#include "tcp.h"
#include <stdint.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

void TCPSocket::waitForConnect()
{
    // connect() is blocking, so once it returns successfully we're connected
    // nothing additional is required here in the client implementation
}

void TCPSocket::connect()
{
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(socket_no);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (::connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        ::close(sockfd);
        sockfd = -1;
    }
}

void TCPSocket::send(const std::string& message)
{
    if (sockfd < 0) return;
    ::send(sockfd, message.c_str(), message.size(), 0);
}

void TCPSocket::receive(char* buffer)
{
    if (sockfd < 0) {
        buffer[0] = '\0';
        return;
    }
    ssize_t len = ::recv(sockfd, buffer, (1<<20) - 1, 0);
    if (len > 0)
        buffer[len] = '\0';
    else
        buffer[0] = '\0';
}

void TCPSocket::shutdown()
{
    if (sockfd != -1) {
        ::shutdown(sockfd, SHUT_RDWR);
        ::close(sockfd);
        sockfd = -1;
    }
}
