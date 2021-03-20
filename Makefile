TARGET=main

INCLUDE_DIR=include
SRC_DIR=src
OBJ_DIR=obj

PKG_CONFIG_PACKAGES=opencv4

CC=g++
CC_FLAGS=-O2 \
		-std=c++11 \
		-Wno-unused-command-line-argument \
		`pkg-config --cflags --libs $(PKG_CONFIG_PACKAGES)` \
		-Iinclude \

# Headers files (include/*.h) in `include` dir
HEADERS=$(wildcard $(INCLUDE_DIR)/*.h)
# Source files (*.cpp) in root path
MAIN_SRC=$(wildcard *.cpp)
# Source files (src/*.cpp) in `src` dir
SRC=$(wildcard $(SRC_DIR)/*.cpp)
# Object files (.o) for source files in root path
MAIN_OBJ=$(MAIN_SRC:.cpp=.o)
# Object files (.o) for source files in `src` dir
SRC_OBJS=$(SRC:$(SRC_DIR)/%.cpp=%.o)
# All object files (.o)
OBJ=$(MAIN_OBJ) $(SRC_OBJS)
# Add path to objects dir to the object files
OBJS=$(addprefix $(OBJ_DIR)/, $(OBJ))

all: objdir $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cpp $(HEADERS)
	$(CC) $(CC_FLAGS) -c $< -o $@

objdir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(TARGET) $(OBJ_DIR)