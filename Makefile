# Variables
CXX = g++
CXXFLAGS = -Wall -g -I$(SRC_DIR)/imgui -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image

# Source files directory
SRC_DIR = src
IMGUI_DIR = $(SRC_DIR)/imgui
OUTPUT_DIR = build
SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(IMGUI_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = my_sdl2_program

# Targets
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $(OUTPUT_DIR)/$(EXEC) $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
