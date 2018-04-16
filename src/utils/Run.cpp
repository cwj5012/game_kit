#include "Run.h"

int main(int argc, char* argv[]) {

    // 初始化 glog
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;

    // 初始化 gtest
    ::testing::InitGoogleTest(&argc, argv);
    // 运行所有 TEST 和 TEST_F 测试
    RUN_ALL_TESTS();
    printf("\n\n");

    // 初始化 google benchmark
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();

    return 0;
}
