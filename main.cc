#include "iterator.hpp"
#include "serializers.hpp"
#include "node.hpp"
#include "visitors.hpp"

template <typename T>
void stepper(Node& node, json& j) {
    MessageT<T> msg;
    from_json(j, msg);
    node.step(msg);
}

int main() {
    JsonInputIterator json_begin(std::cin);
    JsonInputIterator json_end;
    Node node(std::cout);

    for (auto it = json_begin; it != json_end; ++it) {
        nlohmann::json j = *it;
        string type = j["body"]["type"];
        if (type == "echo") {
            stepper<Echo>(node, j);
        } else if (type == "init") {
            stepper<Init>(node, j);
        } else if (type == "generate") {
            stepper<Generate>(node, j);
        } else if (type == "broadcast") {
            stepper<Broadcast>(node, j);
        } else if (type == "read") {
            stepper<Read>(node, j);
        } else if (type == "topology") {
            stepper<Topology>(node, j);
        } else {
            std::cerr << j.dump(4);
        }
    }

    return 0;
}