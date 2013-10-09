#!/usr/local/bin/ruby

require './rsofa_core'

begin
	djm0, djm = RSOFA::cal2jd(2000, 12, 23)
rescue
	puts "error"
else
	if RSOFA::status.nonzero?
		puts "warning"
	else
		puts "Julian Day is #{djm0 + djm}"
	end
end
