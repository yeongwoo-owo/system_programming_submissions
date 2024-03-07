#Makefile
TARGET=2017314333
CXX=gcc
CXXFLAGS=-Wall -g
OBJECTS=2017314333.o scenarioLib.o
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
2017314333.o: 2017314333.c
	$(CXX) $(CXXFLAGS) -c $^
scenarioLib.o: scenarioLib.c
	$(CXX) $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)
