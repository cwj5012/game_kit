#pragma once

#include <string>
#include <memory>

#include <libuv/uv.h>
#include <google/protobuf/message.h>

#include "NonCopyable.h"

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

class ProtobufCodec : public NonCopyable {
public:
    ProtobufCodec() = default;
    ~ProtobufCodec() = default;

    void onMessage(uv_stream_t* stream) {
        
    }

    // 根据二进制字符串，生成 protobuf 消息，失败返回 nullptr
    google::protobuf::Message* createMessage(char* buf) {
        const std::string type_name(buf);
        google::protobuf::Message* message = nullptr;
        const google::protobuf::Descriptor* descriptor =
            google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
        if ( descriptor ) {
            const google::protobuf::Message* proto_type =
                google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
            if ( proto_type ) {
                message = proto_type->New();
            }
        }
        return message;
    }

     MessagePtr parse(const char*buf,int len) {
         MessagePtr message;

    }


private:
    char* buffer_;
    int buffer_len_;
};

