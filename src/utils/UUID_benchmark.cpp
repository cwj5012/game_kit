#include "Run.h"
#include "UUID.h"

using namespace utils;

static void generate_guid(benchmark::State& state) {
	for (auto _ : state) {
		auto u = utils::UUID();
		std::string str;
		u.generateGUID(str);
		EXPECT_EQ(str.size(), utils::GUID_LEN);
	}

}

BENCHMARK(generate_guid);

static void generate_uuid(benchmark::State& state) {
	for (auto _ : state) {
		auto u = utils::UUID();
		std::string str;
		u.generateUUID(str);
		EXPECT_EQ(str.size(), utils::UUID_LEN);
	}
}

BENCHMARK(generate_uuid);
