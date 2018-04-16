#pragma once

/**
 * 禁止类拷贝
 * 
 * 通过继承这个类，来禁止类的拷贝
 */
class NonCopyable {
protected:
	NonCopyable() = default;
	~NonCopyable() = default;

public:
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};
