#ifndef MAEL_NODE
#define MAEL_NODE

#include "serializers.hpp"
#include "visitors.hpp"
#include "format"
#include <cstdio>

#define DEBUG

class Node : public MessageVisitor {
public:
    Node(std::ostream &out): out(out) {}

    template<typename T>
    void step(MessageT<T>& msg) {
        msg.accept(*this);
    }

    template<typename T>
    void send(T& msg) {
        json j;
        to_json(j, msg);
        #ifdef DEBUG 
        fprintf(stderr, "sending: \n");
        std::cerr << j.dump(4) << std::endl;
        #endif
        out << j.dump() << std::endl;
        msg_id += 1;
    }

    void visit(MessageT<Echo>& msg);
    void visit(MessageT<Init>& msg);
    void visit(MessageT<Generate>& msg);
    void visit(MessageT<Broadcast>& msg);
    void visit(MessageT<Read>& msg);
    void visit(MessageT<Topology>& msg);

private:
    template<typename T, typename R, typename Ok>
    void package_reply(MessageT<T>& msg, R& reply, Ok ok);
    string node_id;
    vector<string> nodes;
    int msg_id = 0;
    std::ostream& out;
    std::set<int> messages;
    TopologyT topology;
};

// Generate impl
void Node::visit(MessageT<Generate>& msg) {
    string gen_id = std::format("{}-{}", node_id, msg_id);
    Message<GenerateOk> reply;
    GenerateOk ok;
    ok.gen_id = gen_id;
    ok.typ = "generate_ok";
    package_reply(msg, reply, ok);
    send(reply);
}

// Echo impl
void Node::visit(MessageT<Echo>& msg) {
    Message<EchoOk> reply;
    EchoOk ok;
    ok.typ = "echo_ok";
    ok.echo = msg.body.echo;
    package_reply(msg, reply, ok);
    send(reply);
}

// Init impl
void Node::visit(MessageT<Init>& msg) {
    node_id = msg.body.node_id;
    nodes = msg.body.node_ids;
    Message<InitOk> reply;
    InitOk ok;
    ok.typ = "init_ok";
    package_reply(msg, reply, ok);
    send(reply);
}

// Broadcast impl
void Node::visit(MessageT<Broadcast>& msg) {
    messages.insert(msg.body.message);
    Message<BroadcastOk> reply;
    BroadcastOk ok;
    ok.typ = "broadcast_ok";
    package_reply(msg, reply, ok);
    send(reply);
}

// Read impl
void Node::visit(MessageT<Read>& msg) {
    Message<ReadOk> reply;
    ReadOk ok;
    ok.messages = vector(messages.begin(), messages.end());
    ok.typ = "read_ok";
    package_reply(msg, reply, ok);
    send(reply);
}

// Topology impl
void Node::visit(MessageT<Topology>& msg) {
    topology = msg.body.topology;
    Message<TopologyOk> reply;
    TopologyOk ok;
    ok.typ = "topology_ok";
    package_reply(msg, reply, ok);
    send(reply);
}

template<typename T, typename R, typename Ok>
void Node::package_reply(MessageT<T>& msg, R& reply, Ok ok) {
    fprintf(stderr, "packaging %s reply to msg from %s\n", ok.typ.c_str(), msg.src.c_str());
    ok.in_reply_to = msg.body.id;
    ok.id = msg_id;
    fprintf(stderr, "packaged %s reply\n", ok.typ.c_str());
    reply.dst = msg.src;
    reply.src = node_id;
    reply.body = ok;
    fprintf(stderr, "returning %s reply from %s to %s\n", reply.body.typ.c_str(), reply.src.c_str(), reply.dst.c_str());
}

#endif