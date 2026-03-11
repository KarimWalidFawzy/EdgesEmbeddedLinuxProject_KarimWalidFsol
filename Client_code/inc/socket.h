#ifndef SOCKET_H
#define SOCKET_H
    #if !__cplusplus 
    #error "This program requires C++"
    #endif
    #include <string>
    #include <sys/socket.h>
    #include <unistd.h> // for close()

    // base class for both TCP and UDP sockets
    class Socket
    {
        protected:
        unsigned short int socket_no;   // port number
        int sockfd;                    // underlying file descriptor
        public:
        Socket(int port =8080): socket_no(port), sockfd(-1){}
        virtual ~Socket()
        {
            if (sockfd != -1)
                ::close(sockfd);
        }
        virtual void waitForConnect()=0;
        virtual void connect() = 0;
        virtual void send(const std::string& message) = 0;
        virtual void receive(char* buffer) = 0;
        virtual void shutdown() = 0;
    };
#endif 