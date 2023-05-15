#ifndef MAEL_NODE
#define MAEL_NODE

#include "serializers.hpp"
#include "visitors.hpp"
#include "format"

class Node : public MessageVisitor {
public:
    Node(std::ostream &out): out(out) {}

    template<typename T>
    void step(MessageT<T>& msg) {
        msg.accept(*this);
    }

    template<typename T>
    void send(T msg) {
        json j;
        to_json(j, msg);
        out << j.dump() << std::endl;
        msg_id += 1;
    }

    void visit(MessageT<Echo>& msg);
    void visit(MessageT<Init>& msg);
    void visit(MessageT<Generate>& msg);

private:
    string node_id;
    vector<string> nodes;
    int msg_id = 0;
    std::ostream& out;
};

// Generate impl
void Node::visit(MessageT<Generate>& msg) {
    string gen_id = std::format("{}-{}", node_id, msg_id);
    GenerateOk ok;
    ok.gen_id = gen_id;
    ok.id = msg_id;
    ok.in_reply_to = msg.body.id;
    ok.typ = "generate_ok";
    Message<GenerateOk> reply;
    reply.dst = msg.src;
    reply.src = node_id;
    reply.body = ok;
    send(reply);
}

// Echo impl
void Node::visit(MessageT<Echo>& msg) {
    Message<EchoReply> reply;
    EchoReply ok;
    reply.src = node_id;
    reply.dst = msg.src;
    ok.typ = "echo_ok";
    ok.id = msg_id;
    ok.echo = msg.body.echo;
    ok.in_reply_to = msg.body.id;
    reply.body = ok;
    send(reply);
}

// Init impl
void Node::visit(MessageT<Init>& msg) {
    node_id = msg.body.node_id;
    nodes = msg.body.node_ids;
    InitOk ok;
    ok.typ = "init_ok";
    ok.in_reply_to = msg.body.id;
    Message<InitOk> reply;
    reply.dst = msg.src;
    reply.src = node_id;
    reply.body = ok;
    send(reply);
}

#endif