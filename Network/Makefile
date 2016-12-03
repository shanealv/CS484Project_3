CC = g++

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CC_FLAGS := -std=c++0x -g #-DDEBUG
all: ./obj NetSim

./obj:
	mkdir obj

NetSim: $(OBJ_FILES)
	$(CC) $(CC_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c $^ -o $@

clean:
	rm -f -r obj NetSim

rebuild: clean all

test: all
	./NetSim testGraph.txt 0

run: all
	./NetSim graph.g 123412341234
