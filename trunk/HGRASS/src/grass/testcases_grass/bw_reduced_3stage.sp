* Bandwidth and phase for the three-stage opamp *title
 
VIN 11 0 ac 1
 
 
* Opamp circuit ------------------------------------------------------------------

 
* the first differential input stage
 
* M1 3 10 2 2 nch3 L=0.5u W=8u
rd_1 3 100 170.0000m
rs_1 2 101 170.0000m
capbd_1 100 2 7.4799f
capbs_1 101 2 7.4799f
gm_1 100 101 0 2 397.3752u
rds_1 100 101 733299.112708
cgs_1 0 101 11.9035f
cgd_1 0 100 2.5256f
 
* M2 4 11 2 2 nch3 L=0.5u W=8u
rd_2 4 102 170.0000m
rs_2 2 103 170.0000m
capbd_2 102 2 7.4799f
capbs_2 103 2 7.4799f
gm_2 102 103 11 2 397.3513u
rds_2 102 103 735294.117647
cgs_2 11 103 11.9030f
cgd_2 11 102 2.5256f
 
* M3 3 3 vdd vdd pch3 L=0.5u W=2u
rd_3 3 104 720.0000m
rs_3 0 105 720.0000m
capbd_3 104 0 1.9221f
capbs_3 105 0 1.9221f
gm_3 104 105 3 0 84.0051u
rds_3 104 105 1041277.0555
cgs_3 3 105 3.6906f
cgd_3 3 104 5.429222e-16
 
* M4 4 3 vdd vdd pch3 L=0.5u W=2u
rd_4 4 106 720.0000m
rs_4 0 107 720.0000m
capbd_4 106 0 1.9221f
capbs_4 107 0 1.9221f
gm_4 106 107 3 0 83.9918u
rds_4 106 107 1036420.65081
cgs_4 3 107 3.6906f
cgd_4 3 106 5.429291e-16
 

 

 
* the second stage
 
* M5 6 4 5 5 pch3 L=0.5u W=60u
rd_5 6 108 24.0000m
rs_5 5 109 24.0000m
capbd_5 108 5 52.4874f
capbs_5 109 5 52.4874f
gm_5 108 109 4 5 313.7422u
rds_5 108 109 1595151.25065
cgs_5 4 109 81.7183f
cgd_5 4 108 16.5970f
 
* M6 7 9 5 5 pch3 L=0.5u W=60u
rd_6 7 110 24.0000m
rs_6 5 111 24.0000m
capbd_6 110 5 52.4874f
capbs_6 111 5 52.4874f
gm_6 110 111 9 5 309.7804u
rds_6 110 111 45217.7687744
cgs_6 9 111 85.9490f
cgd_6 9 110 17.6557f
 
* M7 6 6 gnd gnd nch3 L=0.5u W=8u
rd_7 6 112 170.0000m
rs_7 0 113 170.0000m
capbd_7 112 0 7.4799f
capbs_7 113 0 7.4799f
gm_7 112 113 6 0 283.9880u
rds_7 112 113 1022990.16958
cgs_7 6 113 10.8535f
cgd_7 6 112 2.5266f
 
* M8 7 6 gnd gnd nch3 L=0.5u W=8u
rd_8 7 114 170.0000m
rs_8 0 115 170.0000m
capbd_8 114 0 7.4799f
capbs_8 115 0 7.4799f
gm_8 114 115 6 0 294.7787u
rds_8 114 115 1275838.11401
cgs_8 6 115 10.9062f
cgd_8 6 114 2.5261f
 

 

 
* the third CS stage   
 
* M9 12 4 vdd vdd pch3 L=0.5u W=80u
rd_9 12 116 18.0000m
rs_9 0 117 18.0000m
capbd_9 116 0 69.9237f
capbs_9 117 0 69.9237f
gm_9 116 117 4 0 3.4662m
rds_9 116 117 28002.4978228
cgs_9 4 117 150.1204f
cgd_9 4 116 22.1456f
 
* M10 12 7 gnd gnd nch3 L=0.5u W=6.5u
rd_10 12 118 209.2308m
rs_10 0 119 209.2308m
capbd_10 118 0 6.1150f
capbs_10 119 0 6.1150f
gm_10 118 119 7 0 1.3692m
rds_10 118 119 34346.4387895
cgs_10 7 119 11.7976f
cgd_10 7 118 2.0581f
 

 
* bais current branches
 
* M11 9 9 vdd vdd pch3 L=0.5u W=1.15u
rd_11 9 120 1.2522
rs_11 0 121 1.2522
capbd_11 120 0 1.1811f
capbs_11 121 0 1.1811f
gm_11 120 121 9 0 47.3018u
rds_11 120 121 1774228.37477
cgs_11 9 121 2.0958f
cgd_11 9 120 3.074756e-16
 

 
* compensation capacitors 
* default CC1=5p
Cc1 4 8 2.998p
 
Cc2 7 1 20p
* default RC1=600
Rc1 8 12 2445.0
 
Rc2 1 0 5
 

 
* load capacitor 
 
Cl 12 0 30p
 

 
* BW and phase test statement ---------------------------------------------------------------------
 

 

 
* analysis----------------------------------------------
 
.options list node post=2 unwrapp probe
 
.op
 
.ac dec 10 1 1g
 
.print ac VDB(12) VP(12) 
 

 
.END
 
