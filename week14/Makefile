TARGET=ex14
CXX=gcc
CXXFLAGS=-Wall
OBJECT=ex14.o

$(TARGET): $(OBJECT)
			 $(CXX) $(CXXFLAGS) -o $@ $^ -pthread
ex14.o: ex14.c
			 $(CXX) $(CXXFLAGS) -c $^
clean:
	  rm $(OBJECT) $(TARGET)
