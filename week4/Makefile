# Makefile
TARGET=ex4
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=addLine.o addLineNum.o
$(TARGET): $(OBJECTS)
			 $(CXX) $(CXXFLAGS) -o $@ $^ 
addLine.o: addLine.c
			 $(CXX) $(CXXFLAGS) -c $^
addLineNum.o: addLineNum.c
			 $(CXX) $(CXXFLAGS) -c $^
clean:
	  rm $(OBJECTS) $(TARGET)
