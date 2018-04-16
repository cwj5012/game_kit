#pragma once

#include "Run.h"

namespace utils {

/**
 * uint32 to uchar[4]
 * 
 * uint32 范围 0 ~ 4,294,967,295，超出范围部分会被截取
 *  
 *  使用
 *      const uint32_t num = 123456789;
 *      char ch[4];
 *      UInt32ToChar4(num, ch);
 */
inline void UInt32ToChar4(const uint32_t num,
                          char ch[4]) {
    ch[0] = num & 0xFF;
    ch[1] = (num >> 8) & 0xFF;
    ch[2] = (num >> 16) & 0xFF;
    ch[3] = (num >> 24) & 0xFF;
}

/**
 * uchar[4] to uint32
 * 
 * 使用
 *       char ch[4];
 *       uint32_t len;
 *       Char4ToUInt32(ch, len);
 *
 * 备注
 *      这么写正常也能转换，特殊情况没测试
 *      num = *(uint32_t*)ch;
 */
inline void Char4ToUInt32(const char ch[4],
                          uint32_t& num) {
    num = 0;
    num += uint8_t(ch[0]);
    num += uint8_t(ch[1]) << 8;
    num += uint8_t(ch[2]) << 16;
    num += uint8_t(ch[3]) << 24;
}

/**
 * uint16 to uchar[2]
 * 
 * uint16 范围 0 ~ 65,535，超出范围会被截取
 * 
 * 使用
 *       const uint16_t num = 54918;
 *       unsigned char ch[2];
 *       UInt16ToChar2(num, ch);
 */
inline void UInt16ToChar2(const uint16_t num,
                          unsigned char ch[2]) {
    ch[1] = (num >> 8) & 0xFF;
    ch[0] = num & 0xFF;
}

/**
 * uchar[2] to uint16
 * 
 * 使用
 *       unsigned char ch[2];
 *       uint16_t len;
 *       Char2ToUInt16(ch, len);
 *       
 * 备注
 *      这么写正常也能转换，特殊情况没测试
 *      num = *(uint16_t*)ch;
 */
inline void Char2ToUInt16(const unsigned char ch[2],
                          uint16_t& num) {
	num = 0;
	num += uint8_t(ch[0]);
	num += uint8_t(ch[1]) << 8; 
}

/**
 * 取字符数组中前四个字节的字符，转换为数字，获得消息长度
 * 
 * 使用
 *      uint32_t len = 0;
 *      char buf[4];
 *      buf[0] = 1;
 *      buf[1] = 2;
 *      buf[2] = 3;
 *      buf[3] = 4;
 *      GetLengthFromHeader(buf, len);
 */
inline void GetLengthFromHeader(const char* buf,
                                uint32_t& len) {
    char header[4];
    strncpy(header, buf, 4);
    Char4ToUInt32(header, len);
}

}
