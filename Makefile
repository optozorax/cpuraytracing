CXX = g++
CXXFLAGS += -O3 -std=c++11 -w -I . -fpermissive -static-libgcc -fopenmp
OBJECTS = $(patsubst src/%,%, $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)))

all: $(OBJECTS) demo1 demo2 demo3 demo4

demo%: $(OBJECTS) demo%.o
	$(CXX) $(CXXFLAGS) -s $(OBJECTS) $@.o -o $@.exe

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: demo/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# -----------------------------------------------------------------------------

DCXXFLAGS = -g -std=c++11 -w -I . -fpermissive -static-libgcc -fopenmp
DOBJECTS = $(patsubst src/%,d%, $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)))

debug: ddemo1 ddemo2 ddemo3 ddemo4

ddemo%: $(DOBJECTS) ddemo%.o
	$(CXX) $(DCXXFLAGS) $(DOBJECTS) $@.o -o $@.exe

d%.o: src/%.cpp
	$(CXX) $(DCXXFLAGS) -c $< -o $@

d%.o: demo/%.cpp
	$(CXX) $(DCXXFLAGS) -c $< -o $@

# -----------------------------------------------------------------------------

clean:
	rm -f *.o *.out *.ppm