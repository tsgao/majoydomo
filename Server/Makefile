all: Server
newServer.o: newServer.cpp
	g++ newServer.cpp -c
input_output.o: input_output.hpp input_output.cpp
	g++ input_output.cpp -c
Server: newServer.o input_output.o
	g++ input_output.o newServer.o -o Server
clean:
	rm input_output.o newServer.o Server
