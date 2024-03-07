TARGET=pa3.out
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=server.o

$(TARGET):$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread
server.o: server.c
	$(CXX) $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)