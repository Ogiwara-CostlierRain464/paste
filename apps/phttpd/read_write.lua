wrk.method = "POST"
wrk.body = ""
math.randomseed(os.time())
for i = 1, 5 do
	r = math.random(999999)
	wrk.body = wrk.body.."r "..r.."\n"
end
for i = 1, 5 do
	r = math.random(999999)
	wrk.body = wrk.body.."w "..r.." "..r.."\n"
end
wrk.body = wrk.body.."end"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"
