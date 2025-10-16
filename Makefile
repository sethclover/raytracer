CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17

SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

EXEC = $(BUILD_DIR)/raytracer

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(SRC_DIR)/*~ *.ppm

.PHONY: all clean
