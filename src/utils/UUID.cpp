#include "UUID.h"

void utils::UUID::generateGUID(std::string& str) {
	char buf[GUID_LEN];
	CoCreateGuid(&guid_);
	snprintf(buf,
	         sizeof(buf),
	         "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
	         guid_.Data1,
	         guid_.Data2,
	         guid_.Data3,
	         guid_.Data4[0],
	         guid_.Data4[1],
	         guid_.Data4[2],
	         guid_.Data4[3],
	         guid_.Data4[4],
	         guid_.Data4[5],
	         guid_.Data4[6],
	         guid_.Data4[7]);
	str.assign(buf, GUID_LEN);
}

void utils::UUID::generateUUID(std::string& str) {
	char buf[UUID_LEN];
	UuidCreate(&uuid_);
	snprintf(buf,
	         sizeof(buf),
	         "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
	         uuid_.Data1,
	         uuid_.Data2,
	         uuid_.Data3,
	         uuid_.Data4[0],
	         uuid_.Data4[1],
	         uuid_.Data4[2],
	         uuid_.Data4[3],
	         uuid_.Data4[4],
	         uuid_.Data4[5],
	         uuid_.Data4[6],
	         uuid_.Data4[7]);
	str.assign(buf, UUID_LEN);
}
