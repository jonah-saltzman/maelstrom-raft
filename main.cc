#include "iterator.hpp"
#include "message_types.hpp"

class Node {
public:
    Node(std::ostream &out): out(out) {}

    void echo(Message<EchoBody>& msg) {
        Message<EchoReply> reply = {msg.dst, msg.src, EchoReply{"echo_ok", msg_id, msg.body.echo, msg.body.id}};
        json j;
        to_json(j, reply);
        out << j.dump() << std::endl;
        msg_id += 1;
    }

    void init(Message<Init>& msg) {
        id = msg.body.node_id;
        nodes = msg.body.node_ids;
        InitOk ok = {"init_ok", msg.body.id};
        Message<InitOk> reply = {msg.dst, msg.src, ok};
        json j;
        to_json(j, reply);
        out << j.dump() << std::endl;
    }
private:
    string id;
    vector<string> nodes;
    int msg_id = 0;
    std::ostream& out;
};

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
            Message<EchoBody> msg;
            from_json(j, msg);
            node.echo(msg);
        } else if (type == "init") {
            Message<Init> msg;
            from_json(j, msg);
            node.init(msg);
        } else {
            std::cerr << j.dump(4);
        }
    }

    return 0;
}