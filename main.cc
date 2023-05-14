#include "iterator.hpp"
#include "serializers.hpp"
#include "node.hpp"
#include "visitors.hpp"


int main() {
    JsonInputIterator json_begin(std::cin);
    JsonInputIterator json_end;
    Node node(std::cout);

    for (auto it = json_begin; it != json_end; ++it) {
        nlohmann::json j = *it;
        fprintf(stderr, "received: \n");
        std::cerr << j.dump(4) << std::endl;
        string type = j["body"]["type"];
        if (type == "echo") {
            MessageT<Echo> msg;
            from_json(j, msg);
            node.step(msg);
        } else if (type == "init") {
            MessageT<Init> msg;
            from_json(j, msg);
            node.step(msg);
        } else if (type == "generate") {
            MessageT<Generate> msg;
            from_json(j, msg);
            node.step(msg);
        }

        else {
            std::cerr << j.dump(4);
        }
    }

    return 0;
}