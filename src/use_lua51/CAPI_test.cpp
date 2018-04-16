/*
 * lua c api 的操作都在虚拟堆栈上发生，push 入栈，pop 出栈
 * 
 * 栈顶到栈底 -1, -2, -3, -4, -5 ...
 * 栈底到栈顶 1, 2, 3, 4, 5 ...
 *
 * 5   [  top of stack   ]   -1
 * 4   [                 ]   -2
 * 3   [                 ]   -3
 * 2   [                 ]   -4
 * 1   [ bottom of stack ]   -5
 * 
 * 
 * 压入元素 lua_push*
 *
 * void lua_pushnil      (lua_State *L);
 * void lua_pushboolean  (lua_State *L, int bool);
 * void lua_pushnumber   (lua_State *L, lua_Number n);
 * void lua_pushinteger  (lua_State *L, lua_Integer n);
 * void lua_pushlstring  (lua_State *L, const char *s, size_t len);
 * void lua_pushstring   (lua_State *L, const char *s);
 *
 * int lua_checkstack (lua_State *L, int sz);
 *
 * 检查元素
 *
 * lua_is* 检查值是否能转换为指定类型，比如对于任意数字，lua_isstring 都返回真
 * lua_type 返回栈中元素的类型，每种类型对应一个常量，这些常量定义在头文件 lua.h 中
 *
 * 查询元素 lua_to*
 *
 * int          lua_toboolean  (lua_State* L, int index);
 * const char*  lua_tolstring  (lua_State* L, int index, size_t* len);
 * lua_State*   lua_tothread   (lua_State* L, int index);
 * lua_Number   lua_tonumber   (lua_State* L, int index);
 * lua_Integer  lua_tointeger  (lua_State* L, int index);
 * size_t       lua_objlen     (lua_State *L, int idx);
 *
 * 其他栈操作
 *
 * int  lua_gettop		(lua_State *L);
 * void lua_settop		(lua_State *L, int index);
 * void lua_pushvalue	(lua_State *L, int index);
 * void lua_rotate		(lua_State *L, int index, int n);
 * void lua_remove		(lua_State *L, int index);
 * void lua_insert		(lua_State *L, int index);
 * void lua_replace		(lua_State *L, int index);
 * void lua_copy		(lua_State *L, int fromidx, int toidx);
 * 
 */


#include <cstdarg>

#include "Run.h"

#define LUA51

#ifdef LUA51
#pragma comment ( lib,"lua5.1.5.lib" )
extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#endif

#ifdef LUA53
#pragma comment ( lib,"lua5.3.4.lib" )
#include <lua5.3/lua.hpp>
#endif


const std::string value_and_function_lua
	= "C:/Users/cwj/Desktop/game_kit/src/use_lua51/value_and_function.lua";

const std::string call_lib_function_lua
= "C:/Users/cwj/Desktop/game_kit/src/use_lua51/call_lib_function.lua";

/*
 * lua 错误处理函数
 */
void error(lua_State* L, const char* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

/*
 * 打印堆栈信息
 */
static void stackDump(lua_State* L) {
	const int top = lua_gettop(L); /* depth of the stack */
	for (int i = 1; i <= top; i++) {
		/* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
			case LUA_TSTRING: {
				/* strings */
				printf("'%s'", lua_tostring(L, i));
				break;
			}
			case LUA_TBOOLEAN: {
				/* Booleans */
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;
			}
			case LUA_TNUMBER: {
				/* numbers */
				printf("%g", lua_tonumber(L, i));
				break;
			}
			default: {
				/* other values */
				printf("%s", lua_typename(L, t));
				break;
			}
		}
		printf(" "); /* put a separator */
	}
	printf("\n"); /* end the listing */
}

/*
 * 智能调用 lua 函数
 * 
 * d: double
 * i: int
 * s: string
 * >：左侧表示参数，右侧表示返回值
 * 
 * 使用
 *		// 调用 lua 函数 f，参数 x，y 为数字，返回值 z 为数字
 *		call_va(L, "f", "dd>d", x, y, &z)
 */
void call_va(lua_State* L, const char* func, const char* sig, ...) {
	va_list vl;
	int narg = 0; // number of arguments 
	int nres = 0; // number of results

	va_start(vl, sig);
	lua_getglobal(L, func); // push function

	for (narg = 0; *sig; narg++) {
		// repeat for each argument
		// check stack space
		luaL_checkstack(L, 1, "too many arguments");
		switch (*sig++) {
			case 'd': /* double argument */
				lua_pushnumber(L, va_arg(vl, double));
				break;
			case 'i': /* int argument */
				lua_pushinteger(L, va_arg(vl, int));
				break;
			case 's': /* string argument */
				lua_pushstring(L, va_arg(vl, char *));
				break;
			case '>': /* end of arguments */
				goto endargs; /* break the loop */
			default:
				error(L, "invalid option (%c)", *(sig - 1));
		}
	}
endargs:
	nres = strlen(sig); /* number of expected results */
	if (lua_pcall(L, narg, nres, 0) != 0) {
		/* do the call */
		error(L, "error calling '%s': %s", func, lua_tostring(L, -1));
	}
	nres = -nres; /* stack index of first result */
	while (*sig) {
		/* repeat for each result */
		switch (*sig++) {
			case 'd': {
				/* double result */
				const int isnum = lua_isnumber(L, nres);
				if (!isnum) {
					error(L, "wrong result type");
				}
				const double n = lua_tonumber(L, nres);
				*va_arg(vl, double *) = n;
				break;
			}
			case 'i': {
				/* int result */
				const int isnum = lua_isnumber(L, nres);
				if (!isnum) {
					error(L, "wrong result type");
				}
				const int n = lua_tointeger(L, nres);
				*va_arg(vl, int *) = n;
				break;
			}
			case 's': {
				/* string result */
				const char* s = lua_tostring(L, nres);
				if (s == nullptr) {
					error(L, "wrong result type");
				}
				*va_arg(vl, const char **) = s;
				break;
			}
			default:
				error(L, "invalid option (%c)", *(sig - 1));
		}
		nres++;
	}
	va_end(vl);
}

//TEST(CAPI, stack_operate) {
//	lua_State* L = luaL_newstate();
//
//	lua_pushboolean(L, 1);
//	lua_pushnumber(L, 10);
//	lua_pushnil(L);
//	lua_pushstring(L, "hello");
//	stackDump(L); // true 10 nil 'hello'
//
//	lua_pushvalue(L, -4);
//	stackDump(L); // true 10 nil 'hello' true
//
//	lua_replace(L, 3);
//	stackDump(L); // true 10 true 'hello'
//
//	lua_settop(L, 6);
//	stackDump(L); //  true 10 true 'hello' nil nil
//
//	lua_remove(L, -3);
//	stackDump(L); // true 10 true nil nil
//
//	lua_settop(L, -5);
//	stackDump(L); // true
//
//	lua_close(L);
//}


/*
 * 获取变量
 * 
 * 步骤
 *		创建 lua 虚拟机		luaL_newstate
 *		载入 lua 脚本文件		luaL_loadfile
 *		变量入栈				lua_getglobal
 *		检查栈数据类型		lua_is*
 *		访问栈数据			lua_to*
 */
TEST(CAPI,get_lua_value) {
	// 创建 lua 虚拟机
	lua_State* L = luaL_newstate();

	// 打开 lua 默认库
	luaL_openlibs(L);

	// 载入文件
	if (luaL_loadfile(L, value_and_function_lua.c_str())
		|| lua_pcall(L, 0, 0, 0)) {
		error(L, "load file error: %s", lua_tostring(L, -1));
	}

	// 获取 number
	// 获取 width
	lua_getglobal(L, "width"); // 200
	EXPECT_TRUE(lua_isnumber(L, -1));
	EXPECT_EQ("number", luaL_typename(L, -1));
	EXPECT_EQ(200, lua_tonumber(L, -1)); // width

	// 获取 height
	lua_getglobal(L, "height"); // 300
	EXPECT_TRUE(lua_isnumber(L, -1));
	EXPECT_EQ("number", luaL_typename(L, -1));
	EXPECT_EQ(300, lua_tonumber(L, -1)); // height

	// 获取 table
	// 4步操作：表入栈，索引入栈，lua_gettable，lua_to*
	// 获取 white_color["r"]
	lua_getglobal(L, "white_color");
	lua_pushstring(L, "r");
	lua_gettable(L, -2);
	EXPECT_EQ(1, lua_tonumber(L, -1));

	// 获取 white_color["g"]
	lua_getglobal(L, "white_color");
	lua_pushstring(L, "g");
	lua_gettable(L, -2);
	EXPECT_EQ(1, lua_tonumber(L, -1));

	// 获取 white_color["b"]
	lua_getglobal(L, "white_color");
	lua_pushstring(L, "b");
	lua_gettable(L, -2);
	EXPECT_EQ(1, lua_tonumber(L, -1));

	// 获取 table
	// 步操作：表入栈，lua_getfield，lua_to*
	// 获取 black_color["r"]
	lua_getglobal(L, "black_color");
	lua_getfield(L, -1, "r");
	EXPECT_EQ(0, lua_tonumber(L, -1));

	// 获取 black_color["g"]
	lua_getglobal(L, "black_color");
	lua_getfield(L, -1, "g");
	EXPECT_EQ(0, lua_tonumber(L, -1));

	// 获取 black_color["b"]
	lua_getglobal(L, "black_color");
	lua_getfield(L, -1, "b");
	EXPECT_EQ(0, lua_tonumber(L, -1));

	// 调用 lua 函数
	// hello_lua，0 个参数，1 个返回值
	lua_getglobal(L, "hello_lua");
	if (lua_pcall(L, 0, 1, 0) != 0) {
		error(L, "error running function 'hello_lua': %s", lua_tostring(L, -1));
	}
	EXPECT_TRUE(lua_isstring(L, -1));
	EXPECT_STREQ("hello_lua", lua_tostring(L, -1));

	// one，0 个参数，1 个返回值
	lua_getglobal(L, "one");
	if (lua_pcall(L, 0, 1, 0) != 0) {
		error(L, "error running function 'hello_lua': %s", lua_tostring(L, -1));
	}
	EXPECT_TRUE(lua_isnumber(L, -1));
	EXPECT_EQ(1, lua_tonumber(L, -1));

	// sum，2 个参数，1 个返回值
	lua_getglobal(L, "sum");
	lua_pushnumber(L, 10);
	lua_pushnumber(L, 100);
	if (lua_pcall(L, 2, 1, 0) != 0) {
		error(L, "error running function 'hello_lua': %s", lua_tostring(L, -1));
	}
	EXPECT_EQ(110, lua_tonumber(L, -1));

	// 关闭 lua 虚拟机
	lua_close(L);
}


TEST(CAPI,call_va) {
	// 创建 lua 虚拟机
	lua_State* L = luaL_newstate();

	// 打开 lua 默认库
	luaL_openlibs(L);

	// 载入文件
	if (luaL_loadfile(L, value_and_function_lua.c_str())
		|| lua_pcall(L, 0, 0, 0)) {
		error(L, "load file error: %s", lua_tostring(L, -1));
	}

	// int sum(int, int)
	int get_sum;
	call_va(L, "sum", "ii>i", 10, 100, &get_sum);
	EXPECT_EQ(110, get_sum);
	lua_settop(L, 1);

	// char* hello_lua()
	char* get_str = {};
	call_va(L, "hello_lua", ">s", &get_str);
	EXPECT_STREQ("hello_lua", get_str);

	// 关闭 lua 虚拟机
	lua_close(L);
}

/*
 * 注册到 lua 中的函数，具有相同的原型
 * typedef int (*lua_CFunction) (lua_State *L)
 */
static int c_sum(lua_State* L) {
	int a = lua_tonumber(L, 1); // 获得参数
	int b = lua_tonumber(L, 2);
	int sum = a + b;
	lua_pushnumber(L, sum); // c++ 处理的结果，压入堆栈
	return 1; // 结果的数量
}

TEST(CAPI,call_cpp_function) {
	// 创建 lua 虚拟机
	lua_State* L = luaL_newstate();

	// 打开 lua 默认库
	luaL_openlibs(L);

	// 载入文件
	if (luaL_loadfile(L, value_and_function_lua.c_str())
		|| lua_pcall(L, 0, 0, 0)) {
		error(L, "load file error: %s", lua_tostring(L, -1));
	}

	lua_pushcfunction(L, c_sum);
	lua_setglobal(L, "c_sum");

	// call_c_sum，2 个参数，1 个返回值
	lua_getglobal(L, "call_c_sum");
	lua_pushnumber(L, 10);
	lua_pushnumber(L, 100);
	if (lua_pcall(L, 2, 1, 0) != 0) {
		error(L, "error running function 'hello_lua': %s", lua_tostring(L, -1));
	}
	EXPECT_EQ(110, lua_tonumber(L, -1));

	lua_close(L);
}

TEST(CAPI,call_lib_function) {
	// 创建 lua 虚拟机
	lua_State* L = luaL_newstate();

	// 打开 lua 默认库
	luaL_openlibs(L);

	// 载入文件
	if (luaL_loadfile(L, call_lib_function_lua.c_str())
		|| lua_pcall(L, 0, 0, 0)) {
		error(L, "load file error: %s", lua_tostring(L, -1));
	}

	// call_c_sum，2 个参数，1 个返回值
	lua_getglobal(L, "call_mylib_c_sum");
	lua_pushnumber(L, 10);
	lua_pushnumber(L, 100);
	if (lua_pcall(L, 2, 1, 0) != 0) {
		error(L, "error running function 'hello_lua': %s", lua_tostring(L, -1));
	}
	EXPECT_EQ(110, lua_tonumber(L, -1));

	lua_close(L);

}
