CXX = clang++
CXXFLAGS = -std=c++20
SRCS = main.cpp memory_usage.cpp format.cpp
EXE = monitor

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXE)

clean:
	rm $(EXE)