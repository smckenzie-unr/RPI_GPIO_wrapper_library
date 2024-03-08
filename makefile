CXX = aarch64-linux-gnu-g++
CXXFLAGS = -Wall -std=c++17 -O3 -Iinc -fPIC
CLIB = aarch64-linux-gnu-ar rcs
LIBS = -lgpiod

TARGET = aarch64_linux_gpio.a

SRC_DIR := src
BIN_DIR := build

EXE = $(BIN_DIR)/$(TARGET)

C_SRC := $(wildcard $(SRC_DIR)/*.c)
C_OBJ := $(C_SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

CXX_SRC := $(wildcard $(SRC_DIR)/*.cpp)
CXX_OBJ := $(CXX_SRC:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

OBJS = $(CXX_OBJ) $(C_OBJ) 

.PHONY: all clean

all: $(EXE)
	@true

clean:
	@$(RM) -rv $(BIN_DIR) $(BIN_DIR)

$(EXE): $(OBJS)
	@$(CLIB) -o $@ $(OBJS) $(LIBS) $(LD_FLAGS)
	@echo "[\e[0;32mSuccess\e[0m]	Linking complete!"

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "[\e[0;32mSuccess\e[0m]	Compiled "$<" successfully!"

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "[\e[0;32mSuccess\e[0m]	Compiled "$<" successfully!"

$(BIN_DIR):
	@mkdir -p $@
	@echo "[\e[0;32mSuccess\e[0m]	Create build directory."