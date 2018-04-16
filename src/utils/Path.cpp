#include "Path.h"

#include <cstdlib>
#include <Shlwapi.h>

using namespace utils;

string Path::basename(const string& m_path) {
#ifdef _WIN32
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(m_path.c_str(), drive, dir, fname, ext);
	return std::string(drive) + std::string(dir);
#endif
}

//string Path::dirname(const string& m_path) {
//#ifdef _WIN32
//
//#endif
//
//#ifdef Linux
//
//#endif
//	return "";
//}

bool Path::exits(const string& m_path) {
#ifdef _WIN32
	return PathFileExists(m_path.c_str());
#endif

#ifdef Linux

#endif
}

bool Path::isfile(const string& m_path) {
#ifdef _WIN32
	const DWORD dwAttrib = GetFileAttributes(m_path.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES);
#endif
}

bool Path::isdir(const string& m_path) {
#ifdef _WIN32
	const DWORD dwAttrib = GetFileAttributes(m_path.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#endif
}

vector<string> Path::split(const string& m_path) {
#ifdef _WIN32
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(m_path.c_str(), drive, dir, fname, ext);
	vector<string> v;
	v.emplace_back(std::string(drive) + std::string(dir));
	v.emplace_back(std::string(fname));
	v.emplace_back(std::string(ext));
	return v;
#endif
}
