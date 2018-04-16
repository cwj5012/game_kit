#include "Run.h"
#include "Path.h"

using namespace utils;

TEST(Path,base_name) {
	const std::string m1 = "C:/Windows/System32/drivers/etc/host";
	EXPECT_EQ(Path::basename(m1), "C:/Windows/System32/drivers/etc/");
	const std::string m2 = "C:/Windows/regedit.exe";
	EXPECT_EQ(Path::basename(m2), "C:/Windows/");
}

TEST(Path, exits) {
	const std::string m1 = "C:/Windows/System32/drivers/etc/host";
	const std::string m2 = "C:/Windows/System32/drivers/etc";
	// todo
	// EXPECT_TRUE(Path::exits(m1));
	EXPECT_TRUE(Path::exits(m2));
}

TEST(Path, is_file) {
	const std::string m1 = "C:/Windows/System32/drivers/etc/host";
	const std::string m2 = "C:/Windows/System32/drivers/etc";
	// todo
	// EXPECT_TRUE(Path::exits(m1));
	// EXPECT_FALSE(Path::exits(m2));
}

TEST(Path, is_dir) {
	const std::string m1 = "C:/Windows/System32/drivers/etc/host";
	const std::string m2 = "C:/Windows/System32/drivers/etc";
	EXPECT_FALSE(Path::exits(m1));
	EXPECT_TRUE(Path::exits(m2));
}

TEST(Path, split) {
	const std::string m1 = "C:/Windows/System32/drivers/etc/host";
	std::vector<std::string> v1 = Path::split(m1);
	EXPECT_EQ(v1[0], "C:/Windows/System32/drivers/etc/");
	EXPECT_EQ(v1[1], "host");
	EXPECT_EQ(v1[2], "");

	const std::string m2 = "C:/Windows/regedit.exe";
	std::vector<std::string> v2 = Path::split(m2);
	EXPECT_EQ(v2[0], "C:/Windows/");
	EXPECT_EQ(v2[1], "regedit");
	EXPECT_EQ(v2[2], ".exe");
}
