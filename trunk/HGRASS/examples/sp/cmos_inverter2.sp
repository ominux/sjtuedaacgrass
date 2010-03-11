Simple NMOS Inverter
*IODEF: VIN in 1 VOUT out
.lib 'cmos25.lib' tt

*.lib 'mix025.lib' tt

*netlist---------------------------------------

VDD vdd 0 5
VIN in 0 PULSE 0 5 2NS 2NS 2NS 30NS 80NS

RB in base 10k
R2 in base 10k
M1 out base 0 0 nmos L=0.5u W=10u

*M1 out base 0 0 nch L=2u W=100u  **** for 'mix025.lib'

M2 out base vdd vdd pmos L=0.5u W=10u

*extra control information---------------------

*.options post=2 nomod
.options list node post=2 probe
.op

*analysis--------------------------------------

.TRAN 1ns 300ns
.DC VIN 0 5 0.1
.print ac VDB(out) VP(out)
.END

