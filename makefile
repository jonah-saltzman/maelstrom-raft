# Variables
CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -Iexternal/nlohmann_json/single_include/nlohmann
TARGET = test

# Add header files as dependencies
HEADERS = $(wildcard include/*.hpp)

# Rules
all: $(TARGET)

$(TARGET): main.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGET)
