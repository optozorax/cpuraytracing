CXX = g++
CXXFLAGS += -O3 -std=c++11 -w -I . -fpermissive -static-libgcc
OBJECTS = $(patsubst src/%,%, $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)))

all: demo1 demo2 demo3

demo%: $(OBJECTS) demo%.o
	$(CXX) $(CXXFLAGS) -s $(OBJECTS) $@.o -o $@.exe

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: demo/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.out *.ppm