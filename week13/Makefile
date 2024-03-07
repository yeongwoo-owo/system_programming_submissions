#Makefile
TARGET=ex13
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=ex13.o
$(TARGET): $(OBJECTS)
			 $(CXX) $(CXXFLAGS) -o $@ $^ -lpthread
ex13.o: ex13.c
			$(CXX) $(CXXFLAGS) -c $^
clean:
	  rm $(OBJECTS) $(TARGET)
