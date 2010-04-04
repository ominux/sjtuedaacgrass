Simple NMOS Inverter
VIN 20001 0 ac 1
 
 

 
 

 
*netlist---------------------------------------
 

 
 
 

 
RB 20001 20002 10k
 
R2 20001 20002 10k
 
* M1 out base 0 0 nmos L=0.5u W=10u
rd_1 20003 20004 139.5000m
rs_1 0 20005 139.5000m
capbd_1 20004 0 15.6423f
capbs_1 20005 0 15.6423f
gm_1 20004 20005 20002 0 6.3652n
rds_1 20004 20005 7406200471.03
cgs_1 20002 20005 3.1058f
cgd_1 20002 20004 3.1058f
 

 
 

 
* M2 out base vdd vdd pmos L=0.5u W=10u
rd_2 20003 20006 108.5000m
rs_2 0 20007 108.5000m
capbd_2 20006 0 14.4818f
capbs_2 20007 0 14.4818f
gm_2 20006 20007 20002 0 18.2998p
rds_2 20006 20007 464.532912157
cgs_2 20002 20007 17.6934f
cgd_2 20002 20006 17.6934f
 

 
*extra control information---------------------
 

 
*.options post=2 nomod
 
.options list node post=2 probe
 
.op
 

 
*analysis--------------------------------------
 

 
.TRAN 1ns 300ns
 
.DC VIN 0 5 0.1
 
.print ac VDB(20003) VP(20003)
 
.END
 

 
