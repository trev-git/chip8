CXX=g++
CXXFLAGS=-Wall -Wextra -I./raylib/include -L./raylib/lib
LDFLAGS=-lraylib
SRCDIR=./src
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst %.c,%.o,$(SRCS))

main: $(OBJS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o bin/$@ $(LDFLAGS)

clean:
	rm -rf bin compile_commands.json

.PHONY: clean
