// 线程安全的单例类

#pragma once

/**
 * 使用 Singleton::instance()
 */
class Singleton {
public:
	static Singleton& instance() {
		static Singleton instance;
		return instance;
	}

	Singleton(Singleton const&) = delete;
	Singleton& operator=(Singleton const&) = delete;
private:
	Singleton() = default;
};

/**
 * 使用 SingletonBase<int>::instance()
 */
template <typename T>
class SingletonBase {
public:
	static T& instance() {
		static T instance;
		return instance;
	}

	SingletonBase(SingletonBase const&) = delete;
	SingletonBase& operator=(SingletonBase const&) = delete;
protected:
	SingletonBase() = default; // 子类需要访问
};
