all: clean compile link

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Action.o src/Action.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Customer.o src/Customer.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Order.o src/Order.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Volunteer.o src/Volunteer.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/WareHouse.o src/WareHouse.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/main.o src/main.cpp -I include
	

link:
	g++ -o bin/warehouse bin/*.o
# -g -Wall -Weffc++ -std=c++11


clean:
	rm -f bin/*.o bin/wareHouse


# CC = g++
# CFLAGS = -std=c++11 -Wall -g
# #CFLAGS = -g -Wall -Weffc++ -std=c++11 -I

# SRC_DIR = src
# OBJ_DIR = bin
# BIN_DIR = bin
# INCLUDE_DIR = include


# # LIST OF SOURCE files
# SRC_FILES= $(wildcard $(SRC_DIR)/*.cpp)

# #create a list of object files and replace .cpp with .o
# OBJ_FILES= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# #target executable
# TARGET = $(BIN_DIR)/warehouse

# #Build executable
# $(TARGET): $(OBJ_FILES)
# 	$(CC) $(CFLAGS) $^ -o $@

# #build object files
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# #create necessary directories
# $(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# #clean up intermidate and executable files
# clean:
# 	rm -rf $(OBJ_DIR)/*
# 	rm -rf $(BIN_DIR)/warehouse

# # PHONY target to avoid conflict with files name "clean"
# .PHONY: clean


# # # Compiler
# # CXX := g++

# # # Compiler flags
# # CXXFLAGS := -std=c++11 -Wall

# # # Directories
# # SRC_DIR := src
# # INCLUDE_DIR := include
# # BUILD_DIR := build

# # # Source files
# # SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
# # OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# # # Include directories
# # INC := -I$(INCLUDE_DIR)

# # # Target executable
# # TARGET := my_program

# # # List of source files
# # SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
# # OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# # # Rule to build the executable
# # $(TARGET): $(OBJ_FILES)
# # 	$(CXX) $(CXXFLAGS) -o $@ $^ $(INC)


# # # Rule to build the executable
# # # Target executable
# # TARGET := my_program

# # # Rule to build the executable
# # $(TARGET): $(OBJ_FILES)
# # 	$(CXX) $(CXXFLAGS) -o $@ $^ $(INC)  # Include the $(INC) variable here


# # # Clean rule
# # clean:
# # 	rm -rf $(BUILD_DIR) $(TARGET)

# # .PHONY: clean
# # all: clean compile link

# # compile:
# # 	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
# # 	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp
# # 	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Order.o src/Order.cpp
# # 	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp
# # 	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp
# # 	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp


# # link:
# # 	g++ -g -Wall -Weffc++ -std=c++11 -o bin/warehouse bin/*.o

# # clean:
# # 	rm -f bin/*.o
# # 	clear
