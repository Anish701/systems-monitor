CXX = clang++
CXXFLAGS = -std=c++20
SRCS = main.cpp monitor_usage.cpp format.cpp
EXE = monitor

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXE)

clean:
	rm $(EXE)