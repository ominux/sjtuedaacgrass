Simple NMOS Inverter
VIN 20001 0 ac 1
 
 

 
*netlist---------------------------------------
 

 
 
*VIN base 0 PULSE 0 0.9 2NS 2NS 2NS 30NS 80NS
 
 
R1 0 20002 1k
 
CL 20002 0 10p
 
M1 20002 20001 0 0 nch3 L=1u W=20u vgs=900.0000m vds=1.7664 vth=802.0655m vbs=0 vdsat=122.7356m id=33.6310u state=Saturati
rd_1 20002 20003 68.0000m
rs_1 0 20004 68.0000m
capbd_1 20003 0 18.3989f
capbs_1 20004 0 18.3989f
gm_1 20003 20004 20001 0 430.0588u
rds_1 20003 20004 1703119.67243
cgs_1 20001 20004 57.0377f
cgd_1 20001 20003 6.3401f
 

 
*extra control information---------------------
 

 
*.options post=2 nomod
 
.options list node post=2 probe
 
.op
 

 
*analysis--------------------------------------
 

 
*.TRAN 1ns 300ns
 
*.DC VIN 0 5 0.1
 
.print ac VDB(20002) VP(20002)
 
.ac dec 1000 1k 100g
 
.END
 

 
