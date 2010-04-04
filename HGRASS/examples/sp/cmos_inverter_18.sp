Simple NMOS Inverter
*IODEF: VIN base 0 0.9 VOUT out
.lib '/home/madiming/EDA/AnalogCourse/HGRASS/examples/lib/log018.l' tt_3v

*netlist---------------------------------------

VDD vdd 0 1.8
*VIN base 0 PULSE 0 0.9 2NS 2NS 2NS 30NS 80NS
VIN base 0 0.9
*RB in base 1k
*R2 in base 1k
CL out 0 10p
M1 out base 0 0 nch3 L=1u W=5.35u
M2 out base vdd vdd pch3 L=1u W=10u

*extra control information---------------------

*.options post=2 nomod
.options list node post=2 probe
.op

*analysis--------------------------------------

.TRAN 1ns 300ns
.print ac VDB(out) VP(out)
*.DC VIN 0 5 0.1

.END

