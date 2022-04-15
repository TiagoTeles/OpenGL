# Compiler and Linker
CC = g++
LD = g++

# Compiler Flags
CCFLAGS += -g -O0 -std=c++17 
CCFLAGS += -Wall -Wextra -Wpedantic

# GLAD
CCFLAGS += -I $(LIB)/GLAD/include
LDFLAGS += $(LIB)/GLAD/src/glad.o

# GLFW
CCFLAGS += -I $(LIB)/GLFW/include
LDFLAGS += -L $(LIB)/GLFW/lib-mingw-w64
LDFLAGS += -lglfw3 -lopengl32 -lgdi32

# Directories
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = bin
LIB = lib

# Routines
all: libs main clean

libs:
	$(CC) -c lib/GLAD/src/glad.c -o lib/GLAD/src/glad.o -O3 -std=c++17 -I lib/GLAD/include

%.o: %.cpp
	$(CC) -o $@ -c $< $(CCFLAGS)

main: $(OBJ)
	$(LD) -o $(BIN)/main.exe $^ $(LDFLAGS)

clean:
	@powershell "rm src/*.o"
	@powershell "rm lib/GLAD/src/*.o"
