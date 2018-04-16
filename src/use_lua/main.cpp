#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstdarg>

#ifdef __cplusplus
#include <lua5.3/lua.hpp>
#else

#endif


// 根据不同的 lua 版本，使用不同的 C api
#ifdef LUA_VERSION_MAJOR == 5
#ifdef LUA_VERSION_MINOR == 3
#define LUA53
#elif LUA_VERSION_MINOR == 2
#define LUA52
#elif LUA_VERSION_MINOR == 1
#define LUA51
#endif
#endif

static void stackDump(lua_State* L) {
	int top = lua_gettop(L); /* depth of the stack */
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
#ifdef LUA53
			case LUA_TNUMBER: {
				/* numbers */
				if (lua_isinteger(L, i)) /* integer? */
					printf("%lld", lua_tointeger(L, i));
				else /* float */
					printf("%g", lua_tonumber(L, i));
				break;
			}
#else
		case LUA_TNUMBER: {
			/* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
		}
#endif
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

void error(lua_State* L, const char* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

void inputInterpreter() {
	char buff[256];
	int error = 0;
	lua_State* L = luaL_newstate(); /* opens Lua */
	luaL_openlibs(L); /* opens the standard libraries */

	while (fgets(buff, sizeof(buff), stdin) != NULL) {
		error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);
		if (error) {
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1); /* pop error message from the stack */
		}
	}
	lua_close(L);
}

void testStatckOpe() {
	lua_State* L = luaL_newstate();
	lua_pushboolean(L, 1);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	lua_pushstring(L, "hello");
	stackDump(L);
	/* will print: true 10 nil 'hello' */
	lua_pushvalue(L, -4);
	stackDump(L);
	/* will print: true 10 nil 'hello' true */
	lua_replace(L, 3);
	stackDump(L);
	/* will print: true 10 true 'hello' */
	lua_settop(L, 6);
	stackDump(L);
	/* will print: true 10 true 'hello' nil nil */
	lua_rotate(L, 3, 1);
	stackDump(L);
	/* will print: true 10 nil true 'hello' nil */
	lua_remove(L, -3);
	stackDump(L);
	/* will print: true 10 nil 'hello' nil */
	lua_settop(L, -5);
	stackDump(L);
	/* will print: true */
	lua_close(L);
}

int getValueInt(lua_State* L, const char* val) {
	int is_num;
	lua_getglobal(L, val);
	int result = lua_tointegerx(L, -1, &is_num);
	if (!is_num) {
		error(L, "%s should be a number\n", val);

	}
	lua_pop(L, 1); /* remove result from the stack */
	return result;
}

const char* getValueString(lua_State* L, const char* val) {
	lua_getglobal(L, val);
	const char* result = lua_tostring(L, -1);
	int is_string = lua_isstring(L, -1);
	if (!is_string) {
		error(L, "%s should be a string\n", val);
	}
	lua_pop(L, 1); /* remove result from the stack */
	return result;
}

// 访问 lua 变量
void readLuaValue() {
	lua_State* L = luaL_newstate();
	if (luaL_loadfile(L, "c:/Users/cwj/Desktop/source/win/pure_cpp/src/use_lua/windows.lua") || lua_pcall(L, 0, 0, 0)) {
		error(L, "load file error: %s", lua_tostring(L, -1));
	}
	std::cout << "width: " << getValueInt(L, "width") << std::endl;
	std::cout << "height: " << getValueInt(L, "height") << std::endl;
	std::cout << "str: " << getValueString(L, "str") << std::endl;
}

// 调用 lua 函数
int callLuaSum(lua_State* L, int x, int y) {
	int is_num;

	lua_getglobal(L, "LuaSum"); // function to be called
	lua_pushnumber(L, x); // push 1st argument
	lua_pushnumber(L, y); // push 2st argument

	// do the call (2 arguments, 1 result) 
	if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
		error(L, "error running function 'f': %s", lua_tostring(L, -1));
	}

	// retrieve result
	const int z = lua_tonumberx(L, -1, &is_num);

	if (!is_num) {
		error(L, "function 'f' should return a number");
	}

	lua_pop(L, 1); // pop returned value
	return z;
}

// 智能调用 lua 函数
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
	if (lua_pcall(L, narg, nres, 0) != 0) /* do the call */
		error(L, "error calling '%s': %s", func,
		      lua_tostring(L, -1));
	nres = -nres; /* stack index of first result */
	while (*sig) {
		/* repeat for each result */
		switch (*sig++) {
			case 'd': {
				/* double result */
				int isnum;
				double n = lua_tonumberx(L, nres, &isnum);
				if (!isnum)
					error(L, "wrong result type");
				*va_arg(vl, double *) = n;
				break;
			}
			case 'i': {
				/* int result */
				int isnum;
				int n = lua_tointegerx(L, nres, &isnum);
				if (!isnum)
					error(L, "wrong result type");
				*va_arg(vl, int *) = n;
				break;
			}
			case 's': {
				/* string result */
				const char* s = lua_tostring(L, nres);
				if (s == NULL)
					error(L, "wrong result type");
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

// 调用 c 函数
static int c_sin(lua_State* L) {
	int x = lua_tonumber(L, 1);
	lua_pushnumber(L, sin(x));
	return 1;
}

static int c_sum(lua_State* L) {
	int x = lua_tonumber(L, 1);
	int y = lua_tonumber(L, 2);
	lua_pushnumber(L, x + y);
	return 1;
}

// 使用 c 模块
static const struct luaL_Reg mylib[] = {
	{"c_sin", c_sin},
	{"c_sum", c_sum},
	{nullptr, nullptr}
};

int luaopen_mylib(lua_State* L) {
	luaL_newlib(L, mylib);
	return 1;
}

int main(int argc, char* argv[]) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, c_sin);
	lua_setglobal(L, "c_sin");

	lua_pushcfunction(L, c_sum);
	lua_setglobal(L, "c_sum");

	if (luaL_loadfile(L, "C:/Users/cwj/Desktop/game_kit/src/use_lua/windows.lua") || lua_pcall(L, 0, 0, 0)) {
		error(L, "load file error: %s", lua_tostring(L, -1));
	}

	std::cout << "LuaSum(1,2) return " << callLuaSum(L, 1, 2) << std::endl;

	int32_t z;
	call_va(L, "LuaSum", "ii>i", 1, 2, &z);
	std::cout << "LuaSum(1,2) return " << z << std::endl;

	return 0;
}
