#include "Run.h"
#include "UUID.h"

using namespace utils;

// 测试生成的 guid 字符串长度是否正确
TEST(UUID, generate_guid) {
	auto u = utils::UUID();
	std::string str;
	u.generateGUID(str);
	EXPECT_EQ(str.size(), utils::GUID_LEN);
}

TEST(UUID, get_guid) {
	auto u = utils::UUID();
	EXPECT_EQ(u.getGUID().size(), utils::GUID_LEN);
}

// 测试生成的 uuid 字符串长度是否正确
TEST(UUID, generate_uuid) {
	auto u = utils::UUID();
	std::string str;
	u.generateUUID(str);
	EXPECT_EQ(str.size(), utils::UUID_LEN);
}

TEST(UUID, get_uuid) {
	auto u = utils::UUID();
	EXPECT_EQ(u.getUUID().size(), utils::GUID_LEN);
}