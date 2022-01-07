SRC = tests.cpp

CXX = g++ #I used orfeo cluster for compiling.
CXXFLAGS = -Wall -Wextra -std=c++14 -O3 

EXE = $(SRC:.cpp=.x)

# eliminate default suffixes
.SUFFIXES:
SUFFIXES =

# just consider our own suffixes
.SUFFIXES: .cpp .x .o

all: $(EXE)

check: tests.x
	./$< -s

.PHONY: all

%.x:
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $< -o $@ $(CXXFLAGS) -c

format: $(SRC)
	@clang-format -i $^ -verbose || echo "Please install clang-format to run this command"

.PHONY: format

clean:
	rm -f $(EXE) *~ *.o

.PHONY: clean

tests.x : tests_main.o tests.o

tests.o: tests.cpp catch.hpp list_pool.hpp

format : list_pool.hpp
