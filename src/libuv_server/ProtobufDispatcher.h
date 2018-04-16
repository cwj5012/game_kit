#pragma once

#include <memory>
#include <functional>
#include <map>

#include <libuv/uv.h>
#include <google/protobuf/message.h>

#include "NonCopyable.h"


typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

typedef std::function<void(const MessagePtr&)> ProtobufMessageCallback;

class ProtobufDispatcher {
public:
    ProtobufDispatcher() = default;
    ~ProtobufDispatcher() = default;

    void onMessage(MessagePtr message) { }

    void registerCallback() { }
};
