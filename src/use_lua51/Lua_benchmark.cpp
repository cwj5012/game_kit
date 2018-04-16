#include "Run.h"

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}

static void use_global_value(benchmark::State& state) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	const char* buf = "a=1 b=2 function func sum=a+b end";
	for (auto _ : state) {
		luaL_dostring(L, buf);
	}
	lua_close(L);
}

BENCHMARK(use_global_value);

static void use_local_value(benchmark::State& state) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	const char* buf = "local a=1 local b=2 function func local sum=a+b end";
	for (auto _ : state) {
		luaL_dostring(L, buf);
	}
	lua_close(L);
}

BENCHMARK(use_local_value);

static void use_table_value(benchmark::State& state) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	for (auto _ : state) {
		const char* buf = "local t={1,2,3} local a=t[1]";
		luaL_dostring(L, buf);
	}
	lua_close(L);
}

BENCHMARK(use_table_value);

static void use_func_return_value(benchmark::State& state) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	for (auto _ : state) {
		const char* buf = "function num() return 1 end local a=num()";
		luaL_dostring(L, buf);
	}
	lua_close(L);
}

BENCHMARK(use_func_return_value);

static void use_pcall_function(benchmark::State& state) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	for (auto _ : state) {
		const char* buf = "function sum(a,b) return a+b end pcall(sum,1,2)";
		luaL_dostring(L, buf);
	}
	lua_close(L);
}

BENCHMARK(use_pcall_function);

static void use_direct_function(benchmark::State& state) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	for (auto _ : state) {
		const char* buf = "function sum(a,b) return a+b end sum(1,2)";
		luaL_dostring(L, buf);
	}
	lua_close(L);
}

BENCHMARK(use_direct_function);