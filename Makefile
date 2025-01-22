CC = gcc
CFLAGS = -Wall -Wextra -I./include
LDFLAGS = -lSDL3

# Detect OS and set SDL3 include/lib paths
ifeq ($(OS),Windows_NT)
    # Assuming SDL3 is in the project's lib directory
    SDL_DIR = ./lib/SDL3
    CFLAGS += -I$(SDL_DIR)/include
    LDFLAGS += -L$(SDL_DIR)/lib -lmingw32 -lSDL3main
    # Add runtime path for DLL
    RUNTIME_PATH = $(SDL_DIR)/bin
else
    CFLAGS += $(shell sdl3-config --cflags)
    LDFLAGS += $(shell sdl3-config --libs)
endif

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/rebuild_and_resist

all: directories $(EXECUTABLE)
ifeq ($(OS),Windows_NT)
	@if not exist "$(BIN_DIR)\SDL3.dll" copy "$(SDL_DIR)\bin\SDL3.dll" "$(BIN_DIR)"
endif

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean directories
