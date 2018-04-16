#include <iostream>
#include <vector>

#include <glog/logging.h>
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <tuple>
#include <limits>

void InitGlog(char* argv[]) {
	google::InitGoogleLogging(argv[0]);
	google::SetStderrLogging(google::GLOG_INFO);
	FLAGS_colorlogtostderr = true;
}

std::tuple<int, int, int> find() {
	return std::make_tuple(1, 2, 3);
}

void UseTuple() {
	auto [id, name, score] = find();
	LOG(INFO) << id << " " << name << " " << score;
}

void UseMap() {
	std::map<int, int> m;
	auto result = m.insert({10, 100});
	LOG(INFO) << result.second << " " << result.first->second;
}

/**
 * to_string 和 towstring 函数的所有重载
 * 
 * std::string to_string(int value);
 * std::string to_string(long value);
 * std::string to_string(long long value);
 * std::string to_string(unsigned value);
 * std::string to_string(unsigned long value);
 * std::string to_string(unsigned long long value);
 * std::string to_string(float value);
 * std::string to_string(double value);
 * std::string to_string(long double value);
 * 
 * std::wstring to_wstring(int value);
 * std::wstring to_wstring(long value);
 * std::wstring to_wstring(long long value);
 * std::wstring to_wstring(unsigned value);
 * std::wstring to_wstring(unsigned long value);
 * std::wstring to_wstring(unsigned long long value);
 * std::wstring to_wstring(float value);
 * std::wstring to_wstring(double value);
 * std::wstring to_wstring(long double value);
 */
void NumberToString() {
	auto si = std::to_string(42);     // si="42"
	auto sl = std::to_string(42l);    // sl="42"
	auto su = std::to_string(42u);    // su="42"
	auto sd = std::to_string(42.0);   // sd=L"42.000000"
	auto sld = std::to_string(42.0l); // sld=L"42.000000"
}

/**
 * std::ston 格式的函数，其中 n 表示不同类型的整形
 * 
 * std::stoi    string to integer
 * std::stol    string to long
 * std::stoll   string to long long
 * std::stoul   string to unsinged long
 * std::stoull  string to unsinged long long
 */
void StringToInteger() {
	{
		auto i1 = std::stoi("42");                 // i1 = 42
		auto i2 = std::stoi("101010", nullptr, 2); // i2 = 42
		auto i3 = std::stoi("052", nullptr, 8);    // i3 = 42
		auto i4 = std::stoi("0x2A", nullptr, 16);  // i4 = 42
	}

	{
		auto i1 = std::stoi("42");          // i1 = 42
		auto i2 = std::stoi(" 42");         // i2 = 42
		auto i3 = std::stoi(" 42fortytwo"); // i3 = 42
		auto i4 = std::stoi("+42");         // i4 = 42
		auto i5 = std::stoi("-42");         // i5 = -42
	}

	{
		auto i6 = std::stoi("052", nullptr, 8);     // 42
		auto i7 = std::stoi("052", nullptr, 0);     // 42
		auto i8 = std::stoi("0x2A", nullptr, 16);   // 42
		auto i9 = std::stoi("0x2A", nullptr, 0);    // 42
		auto i10 = std::stoi("101010", nullptr, 2); // 42
		auto i11 = std::stoi("22", nullptr, 20);    // 42
		auto i12 = std::stoi("-22", nullptr, 20);   // -42
		auto pos = size_t{0};
		auto i13 = std::stoi("42", &pos);      // i13 = 42   pos = 2
		auto i14 = std::stoi("-42", &pos);     // i14 = -42  pos = 3
		auto i15 = std::stoi(" +42dec", &pos); // i15 = 42   pos = 5
	}
}

/**
 * std::stof 和 std::stod 字符串转浮点数类型
 */
void StringToFloat() {
	{
		auto d1 = std::stof("123.45");       // 123.45000000000000
		auto d2 = std::stod("1.2345e+2");    // 123.4500000000000
		auto d3 = std::stod("0xF.6E6666p3"); // 123.44999980926514
	}

	{
		auto d1 = std::stod("123.45");       // d1 = 123.45000000000000
		auto d2 = std::stod("+123.45");      // d2 = 123.45000000000000
		auto d3 = std::stod("-123.45");      // d3 = -123.45000000000000
		auto d4 = std::stod(" 123.45");      // d4 = 123.45000000000000
		auto d5 = std::stod(" -123.45abc");  // d5 = -123.45000000000000
		auto d6 = std::stod("1.2345e+2");    // d6 = 123.45000000000000
		auto d7 = std::stod("0xF.6E6666p3"); // d7 = 123.44999980926514
		auto d8 = std::stod("INF");          // d8 = inf
		auto d9 = std::stod("-infinity");    // d9 = -inf
		auto d10 = std::stod("NAN");         // d10 = nan
		auto d11 = std::stod("-nanabc");     // d11 = -nan
	}
}

/**
 * std::ston 转换异常处理
 */
void StrongToNumberException() {
	try {
		auto i1 = std::stoi("");
	}
	catch ( std::exception const& e ) {
		// invalid stoi argument
		LOG(INFO) << e.what();
	}

	try {
		auto i2 = std::stoll("12345678901234");
		auto i3 = std::stoi("12345678901234");
	}
	catch ( std::exception const& e ) {
		// stoi argument out of range
		LOG(INFO) << e.what();
	}
}

/**
 * 数字类型的取值范围
 * 
 * template<> class numeric_limits<bool>;
 * template<> class numeric_limits<char>;
 * template<> class numeric_limits<signed char>;
 * template<> class numeric_limits<unsigned char>;
 * template<> class numeric_limits<wchar_t>;
 * template<> class numeric_limits<char16_t>;
 * template<> class numeric_limits<char32_t>;
 * template<> class numeric_limits<short>;
 * template<> class numeric_limits<unsigned short>;
 * template<> class numeric_limits<int>;
 * template<> class numeric_limits<unsigned int>;
 * template<> class numeric_limits<long>;
 * template<> class numeric_limits<unsigned long>;
 * template<> class numeric_limits<long long>;
 * template<> class numeric_limits<unsigned long long>;
 * template<> class numeric_limits<float>;
 * template<> class numeric_limits<double>;
 * template<> class numeric_limits<long double>;
 */
void LimitOfNumber() {
	LOG(INFO).width(10);
	LOG(INFO) << "bool" << std::numeric_limits<bool>::max();
	LOG(INFO).width(10);
	LOG(INFO) << "char" << std::numeric_limits<char>::max();

	LOG(INFO) << "signed char			" << std::numeric_limits<signed char>::max();
	LOG(INFO) << "unsigned char			" << std::numeric_limits<unsigned char>::max();
	LOG(INFO) << "wchar_t				" << std::numeric_limits<wchar_t>::max();
	LOG(INFO) << "char16_t				" << std::numeric_limits<char16_t>::max();
	LOG(INFO) << "char32_t				" << std::numeric_limits<char32_t>::max();
	LOG(INFO) << "short					" << std::numeric_limits<short>::max();
	LOG(INFO) << "unsigned short        " << std::numeric_limits<unsigned short>::max();
	LOG(INFO) << "int					" << std::numeric_limits<int>::max();
	LOG(INFO) << "unsigned int			" << std::numeric_limits<unsigned int>::max();
	LOG(INFO) << "long					" << std::numeric_limits<long>::max();
	LOG(INFO) << "unsigned long			" << std::numeric_limits<unsigned long>::max();
	LOG(INFO) << "long long				" << std::numeric_limits<long long>::max();
	LOG(INFO) << "unsigned long long	" << std::numeric_limits<unsigned long long>::max();
	LOG(INFO) << "float					" << std::numeric_limits<float>::max();
	LOG(INFO) << "double				" << std::numeric_limits<double>::max();
	LOG(INFO) << "long double   " << std::numeric_limits<long double>::max();
}


// Limits and other properties of numberic types
// 数字类型的限制和其他属性
template <typename T, typename I>
T mininum(I const start, I const end) {
	T minval = std::numeric_limits<T>::max();
	for ( auto i = start; i < end; i++ ) {
		if ( *i < minval ) {
			minval = *i;
		}
		return minval;
	}
}

// Expressing time intervals with chrono::duration

#include <chrono>

void TestChrono() {
	{
		// c++11 std::chrono::duration 定义类型：小时，分钟，秒，毫秒，微妙，纳秒
		std::chrono::hours half_day(12);
		std::chrono::minutes half_hour(30);
		std::chrono::seconds half_minute(30);
		std::chrono::milliseconds half_second(500);
		std::chrono::microseconds half_millisecond(500);
		std::chrono::nanoseconds half_microsecond(500);
	}

	{
		// c++14 引入 std::chrono_literals 直接使用时间字面量
		using namespace std::chrono_literals;
		auto half_day = 12h;
		auto half_hour = 30min;
		auto half_minute = 30s;
		auto half_second = 500ms;
		auto half_millisecond = 500us;
		auto half_microsecond = 500ns;
	}

	{
		// 用直接转换，从低精度转换为高精度，比如：12小时转为720分钟
		std::chrono::hours half_day_in_h(12);
		std::chrono::minutes half_day_in_min(half_day_in_h);
		std::cout << half_day_in_h.count() << "h" << std::endl;     //12h
		std::cout << half_day_in_min.count() << "min" << std::endl; //720min
	}

	{
		// 用 std::chrono::duration_cast 从高精度转换为低精度
		using namespace std::chrono_literals;
		const auto total_seconds = 12345s;
		auto hours = std::chrono::duration_cast<std::chrono::hours>(total_seconds);
		auto minutes = std::chrono::duration_cast<std::chrono::minutes>(total_seconds % 1h);
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(total_seconds % 1min);
		std::cout << hours.count() << ':'
			<< minutes.count() << ':'
			<< seconds.count() << std::endl; // 3:25:45
	}

	{
		// c++17 用转换函数 floor()，round()，ceil() 做四舍五入
		using namespace std::chrono_literals;
		const auto total_seconds = 12345s;
		auto m1 = std::chrono::floor<std::chrono::minutes>(total_seconds);
		// 205 min
		auto m2 = std::chrono::round<std::chrono::minutes>(total_seconds);
		// 206 min
		auto m3 = std::chrono::ceil<std::chrono::minutes>(total_seconds);
		// 206 min
		auto sa = std::chrono::abs(total_seconds);
	}

	{
		// 用算数运算、复合赋值和比较运算，修改和比较时间间隔
		using namespace std::chrono_literals;
		auto d1 = 1h + 23min + 45s; // d1 = 5025s
		auto d2 = 3h + 12min + 50s; // d2 = 11570s
		if ( d1 < d2 ) {
			std::cout << d1.count() << " less then " << d2.count() << std::endl;
		}
	}

	{
		// 使用 std::chrono::duration<>，实际时间秒等于 ratio 类型乘以数值
		//
		//  namespace std {
		//      namespace chrono {
		// 		    typedef duration<long long, ratio<1, 1000000000>> nanoseconds;
		// 		    typedef duration<long long, ratio<1, 1000000>> microseconds;
		// 		    typedef duration<long long, ratio<1, 1000>> milliseconds;
		// 		    typedef duration<long long> seconds;
		// 		    typedef duration<int, ratio<60> > minutes;
		// 		    typedef duration<int, ratio<3600> > hours;
		// 	    }
		//  }
        //
		// template<class Rep, class Period = std::ratio<1>> class duration;
		// Rep 一种数值类型，用来表示 Period 的数量
		// Period ratio 类型，表示秒的时间单位
		std::chrono::duration<int, std::ratio<3600>> d1(2);
		std::cout << d1.count() << std::endl;
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d1);
		std::cout << seconds.count() << std::endl;
	}
}

int main(int argc, char* argv[]) {
	InitGlog(argv);

	UseTuple();
	UseMap();
	NumberToString();
	StringToInteger();
	StringToFloat();
	StrongToNumberException();
	LimitOfNumber();

	TestChrono();

	// system("pause");
	return 0;
}
