#include <iostream>
#include <iterator>
#include <sstream>
#include "external/nlohmann_json/single_include/nlohmann/json.hpp"

class JsonInputIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = nlohmann::json;
    using difference_type = nlohmann::json;
    using pointer = nlohmann::json*;
    using reference = nlohmann::json&;
    JsonInputIterator() = default;
    explicit JsonInputIterator(std::istream& is) : input_stream(&is) {
        ++(*this);
    }

    JsonInputIterator& operator++() {
        if (input_stream) {
            std::string line;
            if (std::getline(*input_stream, line)) {
                std::istringstream iss(line);
                iss >> current_json;
            } else {
                input_stream = nullptr;
            }
        }
        return *this;
    }

    JsonInputIterator operator++(int) {
        JsonInputIterator temp(*this);
        ++(*this);
        return temp;
    }

    bool operator==(const JsonInputIterator& other) const {
        return input_stream == other.input_stream;
    }

    bool operator!=(const JsonInputIterator& other) const {
        return !(*this == other);
    }

    const nlohmann::json& operator*() const {
        return current_json;
    }

    const nlohmann::json* operator->() const {
        return &current_json;
    }

private:
    std::istream* input_stream = nullptr;
    nlohmann::json current_json;
};
