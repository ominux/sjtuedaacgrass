Self-Bias Inverter
*IODEF: VIN in inn 1 VOUT out
.lib '/home/madiming/EDA/AnalogCourse/HGRASS/examples/lib/log018.l' tt_3v

*netlist---------------------------------------

VDD vdd 0 1.8
CL out 0 10p
M1 out in 0 0 nch3 L=1u W=5u
M2 out in vdd vdd pch3 L=1u W=10u
M3 base base 0 0 nch3 L=1u W=5u
M4 base base vdd vdd pch3 L=1u W=10u
E1 inn 0 base 0 1
V1 in inn ac 1
*extra control information---------------------

*.options post=2 nomod
.options list node post=2 probe
.op

*analysis--------------------------------------

.TRAN 1ns 300ns
*.DC VIN 0 5 0.1
.print ac VDB(out) VP(out)
*.ac dec 1000 1k 100g
.END

