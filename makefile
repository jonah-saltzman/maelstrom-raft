# Variables
CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -Iexternal/nlohmann_json/single_include/nlohmann
TARGET = test

# Add header files as dependencies
HEADERS = $(wildcard include/*.hpp)

# Add source files
SRCS = main.cc serializers.cc

# Rules
all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

clean:
	rm -f $(TARGET)
