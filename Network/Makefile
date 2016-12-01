CC = g++

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CC_FLAGS := -std=c++0x -g -DDEBUG
all: ./obj main.exe

./obj:
	mkdir obj

main.exe: $(OBJ_FILES)
	$(CC) $(CC_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c $^ -o $@

clean:
	rm -f -r obj main.exe

rebuild: clean all

test: all
	./main.exe testGraph.txt 0
