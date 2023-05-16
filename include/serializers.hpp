#ifndef MAEL_TYPES
#define MAEL_TYPES

#include <string>
#include <optional>
#include <vector>
#include <iostream>
#include <cstdio>
#include <map>
#include <set>
#include "nlohmann/json.hpp"

using std::string;
using std::optional;
using std::vector;
using nlohmann::json;
using TopologyT = std::map<string, std::set<string>>;

struct Body {
    string typ;
};

struct IdBody : Body {
    int id;
};

struct Reply {
    int in_reply_to;
};

struct Echo : IdBody {
    string echo;
};

struct EchoOk : Echo, Reply {};

struct Init : IdBody {
    string node_id;
    vector<string> node_ids;
};

struct InitOk : IdBody, Reply {};

struct Generate : IdBody {};

struct GenerateOk : Reply, IdBody {
    string gen_id;
};

struct Broadcast : IdBody {
    int message;
};

struct BroadcastOk : Reply, IdBody {};

struct Read : IdBody {};

struct ReadOk : Reply, IdBody {
    vector<int> messages;
};

struct Topology : IdBody {
    TopologyT topology;
};

struct TopologyOk : Reply, IdBody {};

template<typename B>
struct Message {
    string src;
    string dst;
    B body;
};


void to_json(json& j, const Body& b);
void to_json(json& j, const IdBody& b);
void to_json(json& j, const Reply& r);
void to_json(json& j, const EchoOk& r);
void to_json(json& j, const GenerateOk& ok);
void to_json(json& j, const BroadcastOk& ok);
void to_json(json& j, const ReadOk& ok);
void to_json(json& j, const TopologyOk& ok);
void to_json(json& j, const InitOk& ok);

void from_json(const json& j, Body& b);
void from_json(const json& j, IdBody& b);
void from_json(const json& j, Reply& r);
void from_json(const json& j, Echo& e);
void from_json(const json& j, Generate& g);
void from_json(const json& j, Broadcast& b);
void from_json(const json& j, Read& r);
void from_json(const json& j, Topology& t);
void from_json(const json& j, Init& i);

// Generic reply to json
template<typename T>
void reply_to_json(json& j, const T& ok) {
    json idbody, reply;
    to_json(idbody, static_cast<const IdBody&>(ok));
    to_json(reply, static_cast<const Reply&>(ok));
    j.update(idbody);
    j.update(reply);
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

#endif