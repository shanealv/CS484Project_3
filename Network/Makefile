CC = g++

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CC_FLAGS := -std=c++0x -g -DDEBUG -MMD
LD_FLAGS := -std=c++0x -g -DDEBUG -MMD
all: ./obj main.exe

./obj:
	mkdir obj

main.exe: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: %.cpp
	$(CC) $(CC_FLAGS) -c $^ -o $@

clean:
	rm -f -r obj

rebuild: clean all

test: all
	./main.exe testGraph.txt 0
