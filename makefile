# C++ compiler
CPP= g++

# Executable name
BIN= twion

# Source and build paths
SRC_DIR= ./src
BUILD_DIR= ./build

# Source files
SRCS= main.cpp vector.cpp curve.cpp cubic.cpp window.cpp control.cpp graphics.cpp editor.cpp shape.cpp shapeeditor.cpp

# Object files to build
OBJS= $(SRCS:%.cpp=%.o)

# Dependencies for each source
DEPS= $(OBJS:%.o=$(BUILD_DIR)/%.d)

# Flags for the compiler
CFLAGS= -Wall -static-libstdc++ -lglfw3 -lopengl32 -lgdi32

# Default path for make install
INSTALL_PATH?=/usr/local

$(BIN) : $(OBJS:%=$(BUILD_DIR)/%)
	$(CPP) $^ -o $@ $(CFLAGS)

-include $(DEPS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CPP) -MMD -c $< -o $@ $(CFLAGS)

.PHONY : clean install
clean :
	rm -rf $(BUILD_DIR)
	rm $(BIN) || rm $(BIN).exe

install : $(BIN)
	cp $(BIN) $(INSTALL_PATH)/bin