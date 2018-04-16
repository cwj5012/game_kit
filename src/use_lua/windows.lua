width = 800
height = 600

str = "hello world"

RED = { red = 1.00, green = 0.00, blue = 0.00 }
GREEN = { red = 0.00, green = 1.00, blue = 0.00 }
BLUE = { red = 0.00, green = 0.00, blue = 1.00 }

background_color = RED

function LuaSum(x, y)
    return x + y
end

print(c_sin(1))
print(c_sum(10, 20))

-- 打印数字，字符串，表格，以逗号隔开
function print3(...)
	local tArg = { ... }
	local function test(tab)
		local s = ""

		if type(tab) == "table" then
			s = s .. "{"
			for i, v in pairs(tab) do
				s = s .. tostring(i) .. ":" .. test(v)
			end
			s = string.sub(s, 0, -2) .. "} "
		else

			s = s .. "'" .. tostring(tab) .. "',"
		end
		return s
	end

	print(test(tArg))
end

-- 基于 print3， 最多打印 10 层堆栈信息
function print4(...)
	local tArg = { ... }
	for i = 1, 10 do
		if debug.getinfo(i) then
			local line = debug.getinfo(i).currentline
			local name = debug.getinfo(i, "n").name
			local source = debug.getinfo(i, "S").source

			if line and name and info then
				print3("stack level: " .. i ..
						" line: " .. line ..
						" name: " .. name ..
						" file: " .. source ..
						" data: ", tArg)
			end
		end
	end
end

-- 基于 print3，打印顶层堆栈信息
function print5(...)
	local tArg = { ... }
	for i = 1, 10 do
		if debug.getinfo(i) then
			local line = debug.getinfo(i).currentline
			local name = debug.getinfo(i, "n").name
			local info = debug.getinfo(i, "S")
			if not (line and name and info) then
				local line = debug.getinfo(i - 1).currentline
				local name = debug.getinfo(i - 1, "n").name
				local info = debug.getinfo(i - 1, "S")
				print3("stack level: " .. i - 1 ..
						" line: " .. line ..
						" name: " .. name ..
						" file: " .. info.source ..
						" data: ", tArg)
				break
			end
		end
	end
end

print3(3)
print4(4)
print5(5)