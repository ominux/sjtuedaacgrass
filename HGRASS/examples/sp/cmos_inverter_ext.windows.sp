Simple NMOS Inverter
 

 
 

 
 

 
*netlist---------------------------------------
 

 
 
 

 
RB in base 10k
 
R2 in base 10k
 
* M1 out base 0 0 nmos L=0.5u W=10u
rd_1 out 100 139.5000m
rs_1 0 101 139.5000m
capbd_1 100 0 15.6423f
capbs_1 101 0 15.6423f
gm_1 100 101 base 0 6.3652n
rds_1 100 101 7406200471.03
cgs_1 base 101 3.1058f
cgd_1 base 100 3.1058f
 

 
 

 
* M2 out base vdd vdd pmos L=0.5u W=10u
rd_2 out 102 108.5000m
rs_2 0 103 108.5000m
capbd_2 102 0 14.4818f
capbs_2 103 0 14.4818f
gm_2 102 103 base 0 18.2998p
rds_2 102 103 464.532912157
cgs_2 base 103 17.6934f
cgd_2 base 102 17.6934f
 

 
*extra control information---------------------
 

 
*.options post=2 nomod
 
.options list node post=2 probe
 
.op
 

 
*analysis--------------------------------------
 

 
.TRAN 1ns 300ns
 
.DC VIN 0 5 0.1
 

 
.END
 

 
