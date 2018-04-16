#pragma once

#include <functional>
#include <string>

#include <glog/logging.h>
#include <gtest/gtest.h>

#include "NonCopyable.h"

typedef std::function<void(const std::string& message)> StringMessageCallback;



// 根据包头解析出消息
class LengthCodec : public NonCopyable {
public:
    explicit LengthCodec(const StringMessageCallback& cb)
        : message_callback_(cb) { }

    void onMessage(char* buf, int len) {
        if ( len > 65536 || len < 0 ) {
            LOG(ERROR) << "无效的消息长度";
        } else { }
    }

private:
    StringMessageCallback message_callback_;
};
