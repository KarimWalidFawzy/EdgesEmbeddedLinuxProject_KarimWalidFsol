#include "udp.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

void UDPSocket::waitForConnect()
{
    // UDP is connectionless; after connect() call the default peer is established
}

void UDPSocket::connect()
{
    sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(socket_no);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // "Connect" the UDP socket so that send/recv use this address by default
    if (::connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect (udp)");
        ::close(sockfd);
        sockfd = -1;
    }
}

void UDPSocket::send(const std::string& message)
{
    if (sockfd < 0) return;
    ::send(sockfd, message.c_str(), message.size(), 0);
}

void UDPSocket::receive(char* buffer)
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

void UDPSocket::shutdown()
{
    if (sockfd != -1) {
        ::close(sockfd);
        sockfd = -1;
    }
}
