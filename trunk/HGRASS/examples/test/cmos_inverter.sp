Simple NMOS Inverter
*IODEF: VIN 2 1 VOUT 4
.lib 'cmos25.lib' tt

*.lib 'mix025.lib' tt

*netlist---------------------------------------

VDD 1 0 5
VIN 2 0 PULSE 0 5 2NS 2NS 2NS 30NS 80NS

RB 2 3 10k
R2 2 3 10k
M1 4 3 0 0 nmos L=0.5u W=10u

*M1 4 3 0 0 nch L=2u W=100u  **** for 'mix025.lib'

M2 4 3 1 1 pmos L=0.5u W=10u

*extra control information---------------------

*.options post=2 nomod
.options list node post=2 probe
.op

*analysis--------------------------------------

.TRAN 1ns 300ns
.DC VIN 0 5 0.1
.print ac VDB(4) VP(4) 

.END

