#pragma once

#include <string>
#include <vector>

namespace utils {

using std::string;
using std::vector;

class Path {
public:
	/**
	 * 获取文件所在路径
	 */
	static string basename(const string& m_path);

	/**
	 * 获取目录名
	 */
	// static string dirname(const string& m_path);

	/*
	 * 检查目录是否存在
	 */
	static bool exits(const string& m_path);

	/*
	 * 检查是否为文件
	 */
	static bool isfile(const string& m_path);

	/*
	* 检查是否为目录
	*/
	static bool isdir(const string& m_path);

	/*
	 * 分割路径，返回上层目录，文件名，扩展名
	 */
	static vector<string> split(const string& m_path);
};

}
