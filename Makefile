CPPFLAGS = -Wall -Wextra -pedantic -O3 -std=c++23 -g -I include

.PHONY = all clean

SRC  := $(wildcard src/*.cpp)

all:
	$(CXX) $(CPPFLAGS) -o zmpst main.cpp $(SRC)

clean:
	rm -f zmpst
