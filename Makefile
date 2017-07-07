CXX = g++
CXXFLAGS += -O3 -std=c++11 -w  -I .
OBJECTS = $(patsubst src/%,%, $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)))
OBJECTS +=  main.o
all: 

all: main

main: $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o main.exe

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.out *.ppm