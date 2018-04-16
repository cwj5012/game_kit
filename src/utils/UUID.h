#pragma once

#include <string>

#include <objbase.h>

#include "Run.h"

#pragma comment(lib,"Rpcrt4.lib")

namespace utils {

const int GUID_LEN = 36;
const int UUID_LEN = 36;

/**
 * 标准的 UUID 格式为：xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx (8-4-4-4-12)
 * 标准的 GUID 格式为：xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx (8-4-4-4-12)
 */
class UUID {
public:
	UUID() = default;

	void generateGUID(std::string& str);

	void generateUUID(std::string& str);

	std::string getGUID(){
		std::string str;
		generateGUID(str);
		return str;
	}

	std::string getUUID() {
		std::string str;
		generateUUID(str);
		return str;
	}

private:
	::GUID guid_;
	::UUID uuid_;
};

}
