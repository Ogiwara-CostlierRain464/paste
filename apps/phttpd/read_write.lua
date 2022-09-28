wrk.method = "POST"
wrk.body = ""
for i = 1, 20 do
	wrk.body = wrk.body.."r "..i.."\n"
end
for i = 1, 20 do
	wrk.body = wrk.body.."w "..i.." "..i.."\n"
end
wrk.body = wrk.body.."r 4"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"
