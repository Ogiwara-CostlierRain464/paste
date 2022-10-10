wrk.method = "POST"
wrk.body = ""

-- https://stackoverflow.com/questions/20154991/generating-uniform-random-numbers-in-lua
local A1, A2 = 727595, 798405  -- 5^17=D20*A1+A2
local D20, D40 = 1048576, 1099511627776  -- 2^20, 2^40
local X1, X2 = 0, 1
function rand()
	local U = X2*A2
	local V = (X1*A2 + X2*A1) % D20
	V = (V*D20 + U) % D40
	X1 = math.floor(V/D20)
	X2 = V - X1*D20
	return V/D40
end

for i = 1, 2 do
	r = math.floor(rand() * 1000000)
	wrk.body = wrk.body.."r "..string.format("%07d", r).."\n"
end
for i = 1, 3 do
	r = math.floor(rand() * 1000000)
	wrk.body = wrk.body.."w "..string.format("%07d", r).." 1".."\n"
end
wrk.body = wrk.body.."end"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"
