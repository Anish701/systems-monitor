CXX = clang++
SRCS = main.cpp memory_usage.cpp
EXE = monitor

all:
	$(CXX) $(SRCS) -o $(EXE)

clean:
	rm $(EXE)