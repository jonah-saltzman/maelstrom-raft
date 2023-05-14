#ifndef MAEL_NODE
#define MAEL_NODE

#include "serializers.hpp"
#include "visitors.hpp"

class Node : public MessageVisitor {
public:
    Node(std::ostream &out): out(out) {}

    template<typename T>
    void step(MessageT<T>& msg) {
        msg.accept(*this);
    }

    void visit(MessageT<Echo>& msg);
    void visit(MessageT<Init>& msg);

private:
    string id;
    vector<string> nodes;
    int msg_id = 0;
    std::ostream& out;
};

void Node::visit(MessageT<Echo>& msg) {
    Message<EchoReply> reply;
    EchoReply ok;
    reply.src = id;
    reply.dst = msg.src;
    ok.typ = "echo_ok";
    ok.id = msg_id;
    ok.echo = msg.body.echo;
    ok.in_reply_to = msg.body.id;
    reply.body = ok;
    json j;
    to_json(j, reply);
    out << j.dump() << std::endl;
    msg_id += 1;
}

void Node::visit(MessageT<Init>& msg) {
    id = msg.body.node_id;
    nodes = msg.body.node_ids;
    InitOk ok;
    ok.typ = "init_ok";
    ok.in_reply_to = msg.body.id;
    Message<InitOk> reply;
    reply.dst = msg.src;
    reply.src = id;
    reply.body = ok;
    json j;
    to_json(j, reply);
    out << j.dump() << std::endl;
}

#endif