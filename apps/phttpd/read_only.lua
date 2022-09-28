wrk.method = "POST"
s = "r 0\n"
wrk.body = s
for i = 1, 1 do
	wrk.body = wrk.body.."r "..i.."\n"
end
wrk.body = wrk.body.."r 4"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"
