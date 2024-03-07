CXX=gcc
CXXFLAGS=-Wall
OBJECTS=server.o client.o

all: server client

server: server.o
			 $(CXX) $(CXXFLAGS) -o $@ $^
client: client.o
			 $(CXX) $(CXXFLAGS) -o $@ $^
server.o: server.c
			 $(CXX) $(CXXFLAGS) -c $^
client.o: client.c
			 $(CXX) $(CXXFLAGS) -c $^
clean:
	  rm $(OBJECTS) server client
