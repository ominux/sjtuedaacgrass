V2 Simple NMOS base Inverter
VIN in 0 ac 1
 
 

 
 

 
*netlist---------------------------------------
 

 
 
 

 
RB in base 10k
 
R2 in base 10k
 
* M1 out base 0 0 nmos L=0.5u W=10u
rd_1 out 10000 139.5000m
rs_1 0 10001 139.5000m
capbd_1 10000 0 15.6423f
capbs_1 10001 0 15.6423f
gm_1 10000 10001 base 0 6.3652n
rds_1 10000 10001 7406200471.03
cgs_1 base 10001 3.1058f
cgd_1 base 10000 3.1058f
 

 
 

 
* M2 out base vdd vdd pmos L=0.5u W=10u
rd_2 out 10002 108.5000m
rs_2 0 10003 108.5000m
capbd_2 10002 0 14.4818f
capbs_2 10003 0 14.4818f
gm_2 10002 10003 base 0 18.2998p
rds_2 10002 10003 464.532912157
cgs_2 base 10003 17.6934f
cgd_2 base 10002 17.6934f
 

 
*extra control information---------------------
 

 
*.options post=2 nomod
 
.options list node post=2 probe
 
.op
 

 
*analysis--------------------------------------
 

 
.TRAN 1ns 300ns
 
.DC VIN 0 5 0.1
 
.print ac VDB(out) VP(out)
 
.END
 

 
