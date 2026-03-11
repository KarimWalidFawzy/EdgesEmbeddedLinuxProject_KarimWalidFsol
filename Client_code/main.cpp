/*
QEMU Emulated terminal application (Client)
Simulates sensor reading via input 
*/
#include "tcp.h"
#include "udp.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[])
{
    /*
    SW flow:
    1. Client tries to establish connection with server */
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <protocol>" << std::endl;
        std::cout << "protocol can be: tcp \n";
        std::cout <<"protocol can be: udp "<<"\n";
        return 1;
    }
    std::string mode = std::string(argv[1]);
    if(mode == "tcp")
    {

    }
    else if (mode == "udp")
    {

    }
    /*
    2. Server confirms connection and sends a message to client to set "initial threshold"
    3. Client receives the message and displays simulated LED status as text
    while( 1)
    {
    Server sends command 
    if (command==get temp)
    {
      Client asks user to input simulated temp value
      User inputs temp value from terminal 
      send to server 
    } 
    else 
    { 
    if (command==set threshold)
    {threshold = new threshold value
    update led status based on new threshold}
    }
    vTaskDelay(1000); // Simulate delay for sensor reading
    }
    */

    return 0;    
}