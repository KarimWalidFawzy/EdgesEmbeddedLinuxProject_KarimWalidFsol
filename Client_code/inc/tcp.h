#ifndef TCP_H
#define TCP_H
    #include "socket.h"
    #include <string>
    #if !__cplusplus 
    #error "This program requires C++"
    #endif
    class TCPSocket : public Socket
    {
        public: 
        TCPSocket(int port = 8080): Socket(port){}
        void waitForConnect() override;
        void connect() override;
        void send(const std::string& message) override;
        void receive(char* buffer) override;
        void shutdown() override;
    };
#endif 