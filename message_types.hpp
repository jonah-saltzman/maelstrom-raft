#include <string>
#include <optional>
#include <vector>
#include <iostream>
#include <cstdio>
#include "external/nlohmann_json/single_include/nlohmann/json.hpp"
using std::string;
using std::optional;
using std::vector;
using nlohmann::json;

struct Body {
    string typ;
};

struct Reply {
    int in_reply_to;
};

struct IdBody : Body {
    int id;
};

struct EchoBody : IdBody {
    string echo;
};

struct EchoReply : EchoBody, Reply {};

struct Init : IdBody {
    string node_id;
    vector<string> node_ids;
};

struct InitOk : Body, Reply {};

template<typename B>
struct Message {
    string src;
    string dst;
    B body;
};

// Reply to json
void to_json(json& j, const Reply& r) {
    j = json{{"in_reply_to", r.in_reply_to}};
}

// Reply from json
void from_json(const json& j, Reply& r) {
    j.at("in_reply_to").get_to(r.in_reply_to);
}

// Body to json
void to_json(json& j, const Body& b) {
    j = json{{"type", b.typ}};
}

// Body from json
void from_json(const json& j, Body& b) {
    j.at("type").get_to(b.typ);
}

// IdBody to json
void to_json(json& j, const IdBody& b) {
    j = json{{"type", b.typ}, {"msg_id", b.id}};
}

// IdBody from json
void from_json(const json& j, IdBody& b) {
    j.at("type").get_to(b.typ);
    j.at("msg_id").get_to(b.id);
}

// Echo to json
void to_json(json& j, const EchoBody& e) {
    json base;
    j = json{{"echo", e.echo}};
    to_json(base, static_cast<const IdBody&>(e));
    j.update(base);
}

// Echo from json
void from_json(const json& j, EchoBody& e) {
    j.at("echo").get_to(e.echo);
    from_json(j, static_cast<IdBody&>(e));
}

// Echo reply to json
void to_json(json& j, const EchoReply& r) {
    json base;
    j = json{{"in_reply_to", r.in_reply_to}};
    to_json(base, static_cast<const EchoBody&>(r));
    j.update(base);
}

// Echo reply from json
void from_json(const json& j, EchoReply& r) {
    j.at("in_reply_to").get_to(r.in_reply_to);
    from_json(j, static_cast<EchoBody&>(r));
}

// Init to json
void to_json(json& j, const Init& i) {
    json base;
    j = json{{"node_id", i.node_id}, "node_ids", i.node_ids};
    to_json(j, static_cast<const IdBody&>(i));
    j.update(base);
}

// Init from json
void from_json(const json& j, Init& i) {
    j.at("node_id").get_to(i.node_id);
    j.at("node_ids").get_to(i.node_ids);
    from_json(j, static_cast<IdBody&>(i));
}

// InitOk to json
void to_json(json& j, const InitOk& ok) {
    json body, reply;
    to_json(body, static_cast<const Body&>(ok));
    to_json(reply, static_cast<const Reply&>(ok));
    j = body;
    j.update(reply);
}

// InitOk from json
void from_json(const json& j, InitOk& ok) {
    from_json(j, static_cast<Body&>(ok));
    from_json(j, static_cast<Reply&>(ok));
}

// Message to json
template <typename BodyType>
void to_json(json& j, const Message<BodyType>& m) {
    json body_json;
    to_json(body_json, m.body);
    j = json{{"src", m.src}, {"dest", m.dst}, {"body", body_json}};
}

// Message from json
template <typename BodyType>
void from_json(const json& j, Message<BodyType>& m) {
    BodyType b;
    json body_json = j["body"];
    from_json(body_json, m.body);
    j.at("src").get_to(m.src);
    j.at("dest").get_to(m.dst);
}
