# QEMU-emulated IoT Device Communication
OOP Design:

---

## Building the Project

### Client

You can build both subprojects with the provided Makefile:

```bash
make client
```

or run the commands manually:

```bash
mkdir -p Client_code/build && cd Client_code/build
cmake ..
cmake --build .
```

The resulting executable will be `Client_code` (or `Client_code.exe` on Windows). Run it from the build directory providing `tcp` or `udp` as the first argument:

```bash
./Client_code tcp
# or
./Client_code udp
```

### Server

You'll need a Qt6 installation and CMake 3.16+.

You can also use the top-level Makefile:

```bash
make server
```

Otherwise build manually:

```bash
mkdir -p Server_code/Server/build && cd Server_code/Server/build
cmake ..
cmake --build .
```

Launch the GUI binary (`Server`) and use the simple buttons in the main window to send `get temp` or `set threshold` commands. In addition to the manual buttons, the server automatically polls the client every second by issuing a `get temp` command. The application listens on port 8080 for both TCP and UDP by default.

---

OOP Design:

Enforce object‐oriented programming (OOP) concepts such as Abstraction, Encapsulation, Inheritance,

and Polymorphism throughout the project.

• Abstract Class: Socket

Responsibilities:

Define common socket behaviors (send, receive, connect, shutdown) with pure virtual

functions.

Functions:

o virtual void waitForConnect() = 0;

o virtual void connect() = 0;

o virtual void send(const std::string\& message) = 0;

o virtual void receive() = 0;

o virtual void shutdown() = 0;

7

• Derived Classes: TCPSocket and UDPSocket

Inherit from Socket and implement protocol-specific behaviors for TCP and UDP

communication.

• Abstract Class: Channel

Data Member:

o Socket\* channelSocket; – A pointer to a Socket object (either TCPSocket or UDPSocket).

Responsibilities:

Handle socket interactions through its channelSocket member.

Functions:

o virtual void start() = 0;

o virtual void stop() = 0;

o virtual void send(const std::string\& message) = 0;

o virtual void receive() = 0;

• Derived Classes: ServerChannel and ClientChannel

Implement specific server and client behaviors by utilizing the channelSocket to manage

communication.

Development Environment:

• QEMU Setup:

The client application will be emulated using QEMU with a terminal interface. The QEMU image

will be produced using YOCTO.

• Qt6 Setup:

Develop the server application using Qt6 to provide an intuitive and interactive GUI.

• Makefile:

Provide a well-structured Makefile to automate the build process for both the server and client

applications.

Tools and Technologies:

• C++ (OOP):

For implementing socket communication and GUI logic while enforcing key OOP principles.

• Qt6:

For building the server GUI application.

• YOCTO:

For producing QEMU image include cpp layer added for it.

• QEMU:

For emulating the client application with a terminal interface.

• Socket Programming:

For implementing TCP and UDP communication between the client and server.

---

## Developer Notes

The codebase shipped originally contained stub declarations for sockets and empty
`tcp.cpp`/`udp.cpp`.  To make the project functional:

1. **Socket hierarchy completed** – `Socket` now holds a file descriptor and a
   virtual destructor; both TCP and UDP implementations use POSIX system calls to
   connect/send/receive and clean up properly.  UDP sockets are "connected" so the
   same APIs as TCP can be used.
2. **Client logic added** – the terminal client reads an initial threshold, enters
   a loop handling `get temp`, `set threshold` and `exit` commands, and prints a
   simulated LED state.  The loop will exit cleanly if the connection closes.
3. **Server GUI enhanced** – the Qt `MainWindow` now builds a simple UI with a
   log widget and two buttons, starts a `QTcpServer` and `QUdpSocket` on port
   8080, and replies to clients.  Incoming temperature strings are logged, and
   threshold changes from the GUI are pushed to the client.  A `QTimer` polls
   every second to automatically send `get temp`.
4. **Build tooling** – a top‑level `Makefile` added, README updated with build
   instructions and notes about polling behaviour.
5. **CMake tweaks** – `Network` component added and linked for Qt networking.
6. **Miscellaneous fixes** – includes added (`<sstream>`, `<unistd.h>`), and
   header guards expanded with port/descriptor handling.

### Suggestions for continuation

* Expand the GUI to implement the four‑tab design with real‑time meter,
  historical graph, configuration controls, and quick‑access buttons as
  originally described.
* Add proper error handling and UI feedback for network failures.
* Create unit tests for `TCPSocket`/`UDPSocket` using loopback sockets.
* Integrate the client into a Yocto/QEMU build environment as per project
  description.  The current code is platform‑independent C++ and should port
  easily.
* Consider separating network code into its own library for reuse.

The modifications above are intended to make the repository buildable and
executable out-of-the-box on a host machine; you can now focus on UI polishing
or Yocto integration without re‑implementing the basic communication logic.



