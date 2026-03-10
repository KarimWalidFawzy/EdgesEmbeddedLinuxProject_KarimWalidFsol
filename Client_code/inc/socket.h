#ifndef SOCKET_H
#define SOCKET_H
    #if !__cplusplus 
    #error "This program requires C++"
    #endif
    #include <string>
    class Socket
    {
        protected:
        unsigned short int socket_no;
        public:
        virtual void waitForConnect()=0;
        virtual void connect() = 0;
        virtual void send(const std::string& message) = 0;
        virtual void receive() = 0;
        virtual void shutdown() = 0;
    };
#endif 