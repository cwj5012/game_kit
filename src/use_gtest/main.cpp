#include <gtest/gtest.h>
#include <numeric>

class Point3D {
public:
	Point3D(): x(0), y(0), z(0) {};
	Point3D(const int a, const int b, const int c) : x(a), y(b), z(c) {};
	void set_x(int x) { this->x = x; }
	void set_y(int y) { this->y = y; }
	void set_z(int z) { this->z = z; }
	int get_x() const { return x; }
	int get_y() const { return y; }
	int get_z() const { return z; }
private:
	int x, y, z;
};

TEST(Point3D, Constructor) {
	auto p1 = Point3D();
	ASSERT_EQ(p1.get_x(), 0);
	ASSERT_EQ(p1.get_y(), 0);
	ASSERT_EQ(p1.get_z(), 0);

	auto p2 = Point3D(1, 2, 3);
	ASSERT_EQ(p2.get_x(), 1);
	ASSERT_EQ(p2.get_y(), 2);
	ASSERT_EQ(p2.get_z(), 3);
}

TEST(Point3D, Set) {
	auto p1 = Point3D();
	p1.set_x(1);
	p1.set_y(2);
	p1.set_z(3);
	ASSERT_EQ(p1.get_x(), 1);
	ASSERT_EQ(p1.get_y(), 2);
	ASSERT_EQ(p1.get_z(), 3);
}

TEST(FirstTestCase, FirstTestName) {
	EXPECT_TRUE(true);
	EXPECT_FALSE(false);
}

TEST(SecondTestCase, SecondTestName) {
	EXPECT_EQ(1, 1);  // equal
	EXPECT_NE(1, 2); // not equal
	EXPECT_LT(1, 2);  // less than
	EXPECT_LE(1, 2); // less than or equal
	EXPECT_LE(1, 1);
	EXPECT_GT(2, 1);  // greater than
	EXPECT_GE(2, 1);  // greater than or equal
	EXPECT_GE(2, 2);

	EXPECT_STREQ("abc", "abc");
	EXPECT_STRNE("abc", "efg");
	EXPECT_STRCASEEQ("ABC", "abc");
	EXPECT_STRCASENE("ABB", "efg");

	EXPECT_FLOAT_EQ(1.9999999f, 1.9999998f);
	EXPECT_DOUBLE_EQ(1.9999999f, 1.9999999f);
	EXPECT_NEAR(1.9999999f, 1.9999998f,0.000001);
}

void ThrowRuntimeError() {
	throw std::runtime_error("error");
}

void NoThrow() {

}

TEST(SecondTestCase, TestThrow) {
	EXPECT_THROW(ThrowRuntimeError(), std::runtime_error);
	EXPECT_ANY_THROW(ThrowRuntimeError());
	EXPECT_NO_THROW(NoThrow());
}

bool OneParam(int num) {
	return num > 0;
}

bool TwoParam(int a, int b) {
	return a > b;
}

bool ThreeParam(int a, int b, int c) {
	return a > b && a > c;
}

TEST(SecondTestCase, TestPred) {
	EXPECT_PRED1(OneParam, 1);
	EXPECT_PRED2(TwoParam, 2, 1);
	EXPECT_PRED3(ThreeParam, 2, 1, 0);
}

TEST(SecondTestCase,AddFailure) {
	// FAIL();
	// ADD_FAILURE();
	// ADD_FAILURE_AT(__FILE__, __LINE__);
}

// 继承测试类
class TestFixture:public ::testing::Test {
protected:
	TestFixture() {
		std::cout << "constructing fixture" << std::endl;
		data.resize(10);
		std::iota(std::begin(data), std::end(data), 1);
	}

	~TestFixture() {
		std::cout << "destroying fixture" << std::endl;
	}

	void SetUp() override {}

	void TearDown() override {}

	std::vector<int> data;
};

TEST_F(TestFixture,TestData) {
	ASSERT_EQ(data.size(), 10);
	ASSERT_EQ(data[0], 1);
	ASSERT_EQ(data[data.size() - 1], data.size());
}

// 设置测试环境，在调用 RUN_ALL_TESTS 前，使用 AddGlobalTestEnvironment 注册环境
class TestEnvironment : public ::testing::Environment {
public:
	void SetUp() override {
		std::cout << "environment setup" << std::endl;
	}

	void TearDown() override {
		std::cout << "environment cleanup" << std::endl;
	}
};

GTEST_API_ int main(int argc, char* argv[]) {
	// 测试参数支持3种形式配置
	// 1. 命令行
	// $ run.exe --gtest_xxx=yyy
	// $ use_gtest.exe --gtest_filter=First*
	// 2. 环境变量
	// export GTEST_XXX = "yyy"
	// export GTEST_FILTER = "First*"
	// 3. 代码
	// ::testing::GTEST_FLAG(xxx) = "yyy";	
	// ::testing::GTEST_FLAG(filter) = "First*";	

	// 测试失败抛出异常
	// ::testing::GTEST_FLAG(throw_on_failure) = true;

	// 设置测试用例过滤器
	// ::testing::GTEST_FLAG(filter) = "*";			        // All test
	// ::testing::GTEST_FLAG(filter) = "First*";	        // First* test
	// ::testing::GTEST_FLAG(filter) = "First*:Second*";	// First* and Second* test
	// ::testing::GTEST_FLAG(filter) = "*-First*";          // All except First* test

	// 设置重复测试次数
	// ::testing::GTEST_FLAG(repeat) = 2;

	// 显示测试列表，不运行测试
	// ::testing::GTEST_FLAG(list_tests) = true;

	// 设置测试失败自动添加断点
	// ::testing::GTEST_FLAG(break_on_failure) = true;

	// 设置输出文本文件
	// ::testing::GTEST_FLAG(output) = "xml:out";

	// 设置输出文本颜色，参数 auto, yes, no
	// ::testing::GTEST_FLAG(color) = "auto";

	// 设置输出文本，包含时间
	// ::testing::GTEST_FLAG(print_time) = true;

	// 注册自定义测试环境
	// ::testing::AddGlobalTestEnvironment(new TestEnvironment{});

	// 初始化 gtest 框架
	::testing::InitGoogleTest(&argc, argv);

	// 运行所有 TEST 和 TEST_F 测试
	RUN_ALL_TESTS();
}

/** output

$ use_gtest.exe
[==========] Running 8 tests from 4 test cases.
[----------] Global test environment set-up.
[----------] 2 tests from Point3D
[ RUN      ] Point3D.Constructor
[       OK ] Point3D.Constructor (0 ms)
[ RUN      ] Point3D.Set
[       OK ] Point3D.Set (0 ms)
[----------] 2 tests from Point3D (2 ms total)

[----------] 1 test from FirstTestCase
[ RUN      ] FirstTestCase.FirstTestName
[       OK ] FirstTestCase.FirstTestName (0 ms)
[----------] 1 test from FirstTestCase (1 ms total)

[----------] 4 tests from SecondTestCase
[ RUN      ] SecondTestCase.SecondTestName
[       OK ] SecondTestCase.SecondTestName (0 ms)
[ RUN      ] SecondTestCase.TestThrow
[       OK ] SecondTestCase.TestThrow (0 ms)
[ RUN      ] SecondTestCase.TestPred
[       OK ] SecondTestCase.TestPred (0 ms)
[ RUN      ] SecondTestCase.AddFailure
[       OK ] SecondTestCase.AddFailure (0 ms)
[----------] 4 tests from SecondTestCase (2 ms total)

[----------] 1 test from TestFixture
[ RUN      ] TestFixture.TestData
constructing fixture
destroying fixture
[       OK ] TestFixture.TestData (2 ms)
[----------] 1 test from TestFixture (5 ms total)

[----------] Global test environment tear-down
[==========] 8 tests from 4 test cases ran. (14 ms total)
[  PASSED  ] 8 tests.

 */
