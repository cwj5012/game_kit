local mylib_path="C:/Users/cwj/Desktop/game_kit/builds/x64/Debug/mylib.dll"  
local load_mylib=package.loadlib(mylib_path,"luaopen_mylib")  
load_mylib()

require "mylib"

function call_mylib_c_sum(a, b)
    return mylib.c_sum(a, b)
end