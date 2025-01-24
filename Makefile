# $(info Source_Files="$(SRC_FILES)")

MAKEFILE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
ENV_PATH = $(MAKEFILE_DIR).env

$(info ENV_PATH="$(ENV_PATH)")

include $(ENV_PATH)
export $(shell sed 's/=.*//' $(ENV_PATH))

# Directories
SRC_DIR = $(PROJECT_DIR)/src
BIN_DIR = $(PROJECT_DIR)/bin

CXX = g++
CXXFLAGS = -std=c++17

# Find all the C++ source files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# The executables will have the same name as the source file but be in the bin directory
EXECUTABLES = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%, $(SRC_FILES))

# Default target
all: $(EXECUTABLES)

# Rule to compile each C++ source file into an executable in the bin directory
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)        # Ensure bin directory exists
	$(CXX) $(CXXFLAGS) -o $@ $^  # Compile with C++17 flag

# Clean up the bin directory (optional)
clean:
	rm -rf $(BIN_DIR)/*
