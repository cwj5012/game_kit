#include "Conv.h"

using namespace utils;

static void uint32_conv_char4(benchmark::State& state) {
    for ( auto _ : state ) {
        const uint32_t test_num = 123456789;
        char char_len[4];
        UInt32ToChar4(test_num, char_len);
        uint32_t int_len;
        Char4ToUInt32(char_len, int_len);
    }
}

BENCHMARK(uint32_conv_char4);

static void uint16_conv_char2(benchmark::State& state) {
    for ( auto _ : state ) {
        const uint16_t test_num = 12345;
        unsigned char char_len[2];
        UInt16ToChar2(test_num, char_len);
        uint16_t int_len;
        Char2ToUInt16(char_len, int_len);
    }
}

BENCHMARK(uint16_conv_char2);

static void get_length_from_header(benchmark::State& state) {
    for ( auto _ : state ) {
        uint32_t len = 0;
        char buf[4];
        buf[0] = 1;
        buf[1] = 2;
        buf[2] = 3;
        buf[3] = 4;
        GetLengthFromHeader(buf, len);
    }
}

BENCHMARK(get_length_from_header);
