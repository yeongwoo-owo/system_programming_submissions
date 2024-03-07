TARGET=ex11
CXX=gcc
CXXFLAGS=-Wall
OBJECT=ex11.o

$(TARGET): $(OBJECT)
			 $(CXX) $(CXXFLAGS) -o $@ $^ -lpthread
ex11.o: ex11.c
			 $(CXX) $(CXXFLAGS) -c $^
clean:
	  rm $(OBJECT) $(TARGET)
