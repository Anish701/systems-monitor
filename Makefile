CXX = clang++
CXXFLAGS = -std=c++20
SRCS = src/main.cpp src/monitor_usage.cpp src/format.cpp
EXE = monitor

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXE)

clean:
	rm $(EXE)