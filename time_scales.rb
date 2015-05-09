require 'celes'
require 'date'

TTMTAI = 32.184 # TT - TAI as TT = TAI + 32.184
Celes::L_G = 6.969290134e-10 # 1 - d(TT)/d(TCG)
DateTime.jd( 43144.5 + 2400000.5 ) # start date for TT and TCG = TAI + 32.184
dut1 = -0.7
date = DateTime.now.to_time.utc.to_datetime
#~ date = DateTime.new( 2009, 01, 01, 0, 0, 0, "+00:00" ).to_time.utc.to_datetime
ta = date.to_time.utc.to_a
time = DateTime.new( ta[5], ta[4], ta[3], 0, 0, 0, ta[9] ).to_time.utc.to_datetime
JD = time.jd

t2000 = DateTime.new( 2000, 01, 01, 12, 0, 0, "+00:00" ).to_time.utc.to_datetime
JD00 = t2000.jd

Y, M, D = date.year, date.month, date.day
d = 367 * Y - ( 7 * ( Y + (( M + 9 ) / 12 ))) / 4 + ( 275 * M ) / 9 + D - 730531
JD00 + d

j = Celes::dtf2d( "UTC", Y, M, D, ta[2], ta[1], date.sec + date.to_time.usec * 1e-6 )
puts "UTC : #{j[0] + j[1]}"

mjd0 = Celes::cal2jd( date.year, date.month, date.day )[0]
mjd1 = Celes::cal2jd( date.year, date.month, date.day )[1]
ajd = mjd0 + mjd1
utc1 = time.ajd.to_f
utc1 = ajd.to_f
utc2 = date.day_fraction.to_f

puts "UTC : #{utc1 + utc2}"

tai1 = Celes::utctai( utc1, utc2 )[0]
tai2 = Celes::utctai( utc1, utc2 )[1]

puts "TAI :#{tai1 + tai2}"
puts "TAI - UTC : #{( tai2 - utc2 ) * 86400.0} seconds"

utc1 = Celes::taiutc( tai1, tai2 )[0]
utc2 = Celes::taiutc( tai1, tai2 )[1]

puts "TAI - UTC : #{( tai2 - utc2 ) * 86400.0} seconds"

tt1 = Celes::taitt( tai1, tai2 )[0]
tt2 = Celes::taitt( tai1, tai2 )[1]

#~ Celes::tttai( tt1, tt2 ).each {| e | p e }
puts "TT : #{tt1 + tt2}"
puts "TT - TAI : #{( tt2 - tai2 ) * 86400.0} seconds"

tai1 = Celes::tttai( tt1, tt2 )[0]
tai2 = Celes::tttai( tt1, tt2 )[1]

puts "TT - TAI : #{( tt2 - tai2 ) * 86400.0} seconds"

65.7768 - (( tai2 - utc2 ) + ( tt2 - tai2 )) * 86400.0 # for dut of 2009, 01, 01

ut11 = Celes::utcut1( utc1, utc2, dut1 )[0]
ut12 = Celes::utcut1( utc1, utc2, dut1 )[1]

puts "UT1 : #{ut11 + ut12}"
puts "UT1 - UTC : #{( ut12 - utc2 ) * 86400.0} seconds"

utc1 = Celes::ut1utc( ut11, ut12, dut1 )[0]
utc2 = Celes::ut1utc( ut11, ut12, dut1 )[1]

puts "UT1 - UTC : #{( ut12 - utc2 ) * 86400.0} seconds"
#~ Celes::ut1tai( ut11, ut12, 0 ).each {| e | p e }

dt = Celes::dat( date.year, date.month, date.day, date.day_fraction ) + TTMTAI + dut1

tai1 = Celes::ut1tai( ut11, ut12, dt - TTMTAI )[0]
tai2 = Celes::ut1tai( ut11, ut12, dt - TTMTAI )[1]

puts "UT1 - TAI: #{( ut12 - tai2) * 86400.0} seconds"

ut11 = Celes::taiut1( tai1, tai2, dt - TTMTAI )[0]
ut12 = Celes::taiut1( tai1, tai2, dt - TTMTAI )[1]

puts "UT1 - TAI: #{( ut12 - tai2) * 86400.0} seconds"

tt1 = Celes::ut1tt( ut11, ut12, dt )[0]
tt2 = Celes::ut1tt( ut11, ut12, dt )[1]

puts "TT - UT1 : #{( tt2 - ut12 )  * 86400.0} seconds"

ut11 = Celes::ttut1( tt1, tt2, dt )[0]
ut12 = Celes::ttut1( tt1, tt2, dt )[1]

puts "TT - UT1 : #{( tt2 - ut12 )  * 86400.0} seconds"

gastr = Celes::gst06a( ut11, ut12, tt1, tt2 )

gast = Celes::a2tf( 3, gastr )#[1]#.each {| e | p e }

printf("%4d/%2.2d/%2.2d%3d:%2.2d:%2.2d.%3.3d\n", 
Y, M, D, gast[1][0], gast[1][1], gast[1][2], gast[1][3]) 


#~ Celes::jd2cal( tt1, tt2 ).each {| e | p e }
#~ Celes::d2tf( 3, Celes::jd2cal( tt1, tt2 )[3] )[1].each {| e | p e }
#~ Celes::d2dtf( "UTC", 3, tt1, tt2 )[3].each {| e | p e }
#~ Celes::jdcalf( 3, tt1, tt2 ).each {| e | p e }
#~ Celes::num06a( tt1, tt2 )[0].each {| e | p e }
#~ Celes::num06a( tt1, tt2 )[1].each {| e | p e }
#~ Celes::num06a( tt1, tt2 )[2].each {| e | p e }
#~ Celes::a2af( 3, Celes::obl06( tt1, tt2 ))[1].each {| e | p e }
#~ Celes::p06e( tt1, tt2 ).each {| e | p e }
#~ Celes::d2dtf( "UTC", 3, mjd0, mjd1 + date.day_fraction.to_f )


#~ p 43144.0 + mjd0 + 32.184 / 86400.0
#~ p DateTime.mjd( 43144.0 + mjd0 + 32.184 / 86400.0 )
