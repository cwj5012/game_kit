#include "Conv.h"

using namespace utils;

// 测试 uint32 和 char[4] 之间的转换
TEST(Conv, uint32_conv_char4) {
    const uint32_t test_num = 123456789; // 4个字节以 uint8 表示 [7] [91] [205] [21]
    char char_len[4];
    UInt32ToChar4(test_num, char_len);
    uint32_t int_len;
    Char4ToUInt32(char_len, int_len);
    EXPECT_EQ(int_len, 123456789);
}

TEST(Conv, uint16_conv_char2) {
    const uint16_t test_num = 54918;
    unsigned char char_len[2];
    UInt16ToChar2(test_num, char_len);
    uint16_t int_len;
    Char2ToUInt16(char_len, int_len);
    EXPECT_EQ(int_len, 54918);
}

TEST(Conv, get_length_from_header) {
    uint32_t len = 0;
    char buf[4];
    buf[0] = 1;
    buf[1] = 2;
    buf[2] = 3;
    buf[3] = 4;
    GetLengthFromHeader(buf, len);
    EXPECT_EQ(len, 1 + 2 * pow(2, 8) + 3 * pow(2, 16) + 4 * pow(2, 24));
}

