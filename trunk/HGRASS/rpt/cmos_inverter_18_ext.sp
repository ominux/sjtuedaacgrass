Simple NMOS Inverter
VIN 20001 0 ac 0.9
 
 

 
*netlist---------------------------------------
 

 
 
*VIN base 0 PULSE 0 0.9 2NS 2NS 2NS 30NS 80NS
 
 
*RB in base 1k
 
*R2 in base 1k
 
CL 20002 0 10p
 
* M1 out base 0 0 nch3 L=1u W=5.35u
rd_1 20002 20003 254.2056m
rs_1 0 20004 254.2056m
capbd_1 20003 0 5.0686f
capbs_1 20004 0 5.0686f
gm_1 20003 20004 20001 0 105.1838u
rds_1 20003 20004 182785.282129
cgs_1 20001 20004 15.3988f
cgd_1 20001 20003 1.9171f
 
* M2 out base vdd vdd pch3 L=1u W=10u
rd_2 20002 20005 144.0000m
rs_2 0 20006 144.0000m
capbd_2 20005 0 8.8966f
capbs_2 20006 0 8.8966f
gm_2 20005 20006 20001 0 74.1743u
rds_2 20005 20006 13961001.3389
cgs_2 20001 20006 34.1755f
cgd_2 20001 20005 2.7553f
 

 
*extra control information---------------------
 

 
*.options post=2 nomod
 
.options list node post=2 probe
 
.op
 

 
*analysis--------------------------------------
 

 
.TRAN 1ns 300ns
 
.print ac VDB(20002) VP(20002)
 
*.DC VIN 0 5 0.1
 

 
.END
 

 
