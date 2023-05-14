#ifndef MAEL_VISIT
#define MAEL_VISIT

#include "serializers.hpp"

class MessageVisitor;

class MessageBase {
public:
    virtual void accept(MessageVisitor& visitor) = 0;
};

template<typename B>
class MessageT : public Message<B>, public MessageBase {
public:
    void accept(MessageVisitor& visitor) override;
};

class MessageVisitor {
public:
    virtual void visit(MessageT<Echo>& msg) = 0;
    virtual void visit(MessageT<Init>& msg) = 0;
    // Add visit methods for other message types
};

template <typename B>
void MessageT<B>::accept(MessageVisitor& visitor) {
    visitor.visit(*this);
}

#endif