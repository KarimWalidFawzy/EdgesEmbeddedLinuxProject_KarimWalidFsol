# simple makefile to build both client and server using cmake

.PHONY: all client server clean

all: client server

client:
	cd Client_code && mkdir -p build && cd build && cmake .. && cmake --build .

server:
	cd Server_code/Server && mkdir -p build && cd build && cmake .. && cmake --build .

clean:
	rm -rf Client_code/build
	rm -rf Server_code/Server/build
