wrk.method = "POST"
s = "r 0\n"
wrk.body = s
for i = 1, 20 do
	wrk.body = wrk.body.."r "..i.."\n"
end
wrk.body = wrk.body.."end"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"
