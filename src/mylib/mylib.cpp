#define LUA51

#ifdef LUA51
extern "C" {
#endif
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
#ifdef LUA51
}
#endif

#pragma comment(lib, "lua5.1.5.lib")

/*
* ע�ᵽ lua �еĺ�����������ͬ��ԭ��
* typedef int (*lua_CFunction) (lua_State *L)
*/
static int c_sum(lua_State* L) {
	int a = lua_tonumber(L, 1); // ��ò���
	int b = lua_tonumber(L, 2);
	int sum = a + b;
	lua_pushnumber(L, sum); // c++ ����Ľ����ѹ���ջ
	return 1; // ���������
}

static const struct luaL_Reg mylib[] = {
	{"c_sum", c_sum},
	{nullptr, nullptr}
};

#ifdef _WIN32 
extern "C" __declspec(dllexport) int luaopen_mylib(lua_State* L) {
#else
extern "C"  int luaopen_mylib(lua_State* L) {
#endif // _WIN32 
	luaL_register(L, "mylib", mylib);
//	luaL_openlib(L, "mylib", mylib, 0);
	return 1;
}
