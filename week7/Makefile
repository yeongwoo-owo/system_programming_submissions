#Makefile

TARGET=ex7
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=ex7.o

$(TARGET): $(OBJECTS)
			 $(CXX) $(CXXFLAGS) -o $@ $^
ex7.o: ex7.c
		$(CXX) $(CXXFLAGS) -c $^
clean:
	  rm $(OBJECTS) $(TARGET)
