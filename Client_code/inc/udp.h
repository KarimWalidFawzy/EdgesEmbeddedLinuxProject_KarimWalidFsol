#ifndef UDP_H
#define UDP_H
    #include "socket.h"
    #if !__cplusplus 
    #error "This program requires C++"
    #endif
    class UDPSocket : public Socket
    {
        public:
        UDPSocket(int port = 8080): Socket(port){}
        void waitForConnect() override;
        void connect() override;
        void send(const std::string& message) override;
        void receive() override;
        void shutdown() override;
    };
#endif 