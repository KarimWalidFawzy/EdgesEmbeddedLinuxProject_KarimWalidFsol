/*
QEMU Emulated terminal application (Client)
Simulates sensor reading via input 
*/
#include "tcp.h"
#include "udp.h"
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include "socket.h"
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
    Socket* clientSocket;    
    if(mode == "tcp")
    {
        clientSocket = new TCPSocket();

    }
    else if (mode == "udp")
    {
        clientSocket = new UDPSocket();
    }
    else
    {
        std::cerr << "Invalid protocol specified. Use 'tcp' or 'udp'." << std::endl;
        return 1;
    }
    clientSocket->connect();
    clientSocket->waitForConnect();
            
    /*
    2. Server confirms connection and sends a message to client to set "initial threshold"
    3. Client receives the message and displays simulated LED status as text
    */
   char buffer[1<<20];
   clientSocket->receive(buffer);
   std::cout << "Received from server: " << buffer << std::endl;

   // interpret the first message as initial threshold
   int threshold = 0;
   try {
       threshold = std::stoi(buffer);
   } catch (...) {
       std::cerr << "Invalid threshold received" << std::endl;
   }
   std::cout << "Starting with threshold " << threshold << std::endl;

   // helper to display led status based on last temperature
   auto printLed = [&](int lastTemp) {
       if (lastTemp > threshold)
           std::cout << "LED Status: ON (" << lastTemp << " > " << threshold << ")\n";
       else
           std::cout << "LED Status: OFF (" << lastTemp << " <= " << threshold << ")\n";
   };

   // interactive loop: respond to commands from server
   bool running = true;
   int lastTemp = 0;
   while (running) {
       // wait for next command
       clientSocket->receive(buffer);
       std::string command(buffer);
       if (command.empty()) {
           // connection closed or error
           break;
       }
       std::cout << "Command from server: '" << command << "'\n";
       if (command == "get temp") {
           std::cout << "Enter simulated temperature: ";
           std::string temp;
           std::getline(std::cin, temp);
           clientSocket->send(temp);
           try {
               lastTemp = std::stoi(temp);
           } catch (...) {}
           printLed(lastTemp);
       } else if (command.rfind("set threshold", 0) == 0) {
           // format: set threshold <value>
           std::istringstream iss(command);
           std::string tok;
           int newTh = threshold;
           iss >> tok >> tok >> newTh;
           threshold = newTh;
           std::cout << "New threshold: " << threshold << std::endl;
           printLed(lastTemp);
       } else if (command == "exit") {
           running = false;
       } else {
           std::cout << "Unknown command, ignoring." << std::endl;
       }
   }

   clientSocket->shutdown();
   delete clientSocket;
   return 0;    
}