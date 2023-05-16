#include "serializers.hpp"

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

// Echo from json
void from_json(const json& j, Echo& e) {
    j.at("echo").get_to(e.echo);
    from_json(j, static_cast<IdBody&>(e));
}

// Echo reply to json
void to_json(json& j, const EchoOk& r) {
    json base;
    j = json{{"in_reply_to", r.in_reply_to}};
    to_json(base, static_cast<const Echo&>(r));
    j.update(base);
}

// Generate reply to json
void to_json(json& j, const GenerateOk& ok) {
    j = json{{"id", ok.gen_id}};
    reply_to_json(j, ok);
}

// Broadcast from json
void from_json(const json& j, Broadcast& b) {
    j.at("message").get_to(b.message);
    from_json(j, static_cast<IdBody&>(b));
}

// BroadcastOk to json
void to_json(json& j, const BroadcastOk& ok) {
    reply_to_json(j, ok);
}

// Read from json
void from_json(const json& j, Read& r) {
    from_json(j, static_cast<IdBody&>(r));
}

// ReadOk to json
void to_json(json& j, const ReadOk& ok) {
    j = json{{"messages", ok.messages}};
    reply_to_json(j, ok);
}

// Topology from json
void from_json(const json& j, Topology& t) {
    json json_topo = j["topology"];
    for(auto& [key, value] : json_topo.items()) {
        for(auto& item : value) {
            t.topology[key].insert(item);
        }
    }
    from_json(j, static_cast<IdBody&>(t));
}

// TopologyOk to json
void to_json(json& j, const TopologyOk& ok) {
    reply_to_json(j, ok);
    std::cerr << j.dump(4) << std::endl;
}

// Generate from json
void from_json(const json& j, Generate& g) {
    from_json(j, static_cast<IdBody&>(g));
}

// Init from json
void from_json(const json& j, Init& i) {
    j.at("node_id").get_to(i.node_id);
    j.at("node_ids").get_to(i.node_ids);
    from_json(j, static_cast<IdBody&>(i));
}

// InitOk to json
void to_json(json& j, const InitOk& ok) {
    reply_to_json(j, ok);
}