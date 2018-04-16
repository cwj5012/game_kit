#include "Run.h"

int main(int argc, char* argv[]) {

    // ��ʼ�� glog
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;

    // ��ʼ�� gtest
    ::testing::InitGoogleTest(&argc, argv);
    // �������� TEST �� TEST_F ����
    RUN_ALL_TESTS();
    printf("\n\n");

    // ��ʼ�� google benchmark
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();

    return 0;
}
