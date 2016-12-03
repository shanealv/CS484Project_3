CC = g++

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CC_FLAGS := -std=c++0x -g -DDEBUG -MMD
LD_FLAGS := -std=c++0x -g -DDEBUG -MMD
all: ./obj generator

./obj:
	mkdir obj

generator: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: %.cpp
	$(CC) $(CC_FLAGS) -c $^ -o $@

clean:
	rm -f -r obj generator graph.g

rebuild: clean all

run: all
	./generator
