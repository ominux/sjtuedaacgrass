*3stage OPAMP original
 
* the first differential input stage
 
M1 3 12 2 2 nch3 L=0.5e-6 W=8e-6
rd_1 3 100 170e-3
rs_1 2 101 170e-3
capbd_1 100 2 7.4799e-15
capbs_1 101 2 7.4799e-15
gm_1 100 101 0 2 397.8797e-6
rds_1 100 101 731903.681476
cgs_1 0 101 11.9069e-15
cgd_1 0 100 2.5256e-15
 
M2 4 13 2 2 nch3 L=0.5e-6 W=8e-6
rd_2 4 102 170.0000e-3
rs_2 2 103 170.0000e-3
capbd_2 102 2 7.4799e-15
capbs_2 103 2 7.4799e-15
gm_2 102 103 13 2 397.8516e-6
rds_2 102 103 734268.301637
cgs_2 13 103 11.9063e-15
cgd_2 13 102 2.5256e-15
 
M3 3 3 0 0 pch3 L=0.5e-6 W=2e-6
rd_3 3 104 720.0000e-3
rs_3 0 105 720.0000e-3
capbd_3 104 0 1.9221e-15
capbs_3 105 0 1.9221e-15
gm_3 104 105 3 0 84.0722e-6
rds_3 104 105 1039610.94432
cgs_3 3 105 3.6908e-15
cgd_3 3 104 5.429223e-16

M4 4 3 0 0 pch3 L=0.5e-6 W=2e-6
rd_4 4 106 720.0000e-3
rs_4 0 107 720.0000e-3
capbd_4 106 0 1.9221e-15
capbs_4 107 0 1.9221e-15
gm_4 106 107 3 0 84.0566e-6
rds_4 106 107 1033911.04147
cgs_4 3 107 3.6908e-15
cgd_4 3 106 5.429304e-16
 
M5 2 1 0 0 nch3 L=0.5e-6 W=25e-6
rd_5 2 108 54.4000e-3
rs_5 0 109 54.4000e-3
capbd_5 108 0 22.9485e-15
capbs_5 109 0 22.9485e-15
gm_5 108 109 1 0 909.7460e-6
rds_5 108 109 183725.587463
cgs_5 1 109 34.4548e-15
cgd_5 1 108 7.9234e-15
 

 
* the second stage
 
M6 6 4 5 5 pch3 L=0.5e-6 W=60e-6
rd_6 6 110 24.0000e-3
rs_6 5 111 24.0000e-3
capbd_6 110 5 52.4874e-15
capbs_6 111 5 52.4874e-15
gm_6 110 111 4 5 314.0341e-6
rds_6 110 111 1593477.70454
cgs_6 4 111 81.7473e-15
cgd_6 4 110 16.5970e-15
 
M7 7 9 5 5 pch3 L=0.5e-6 W=60e-6
rd_7 7 112 24.0000e-3
rs_7 5 113 24.0000e-3
capbd_7 112 5 52.4874e-15
capbs_7 113 5 52.4874e-15
gm_7 112 113 9 5 310.0655e-6
rds_7 112 113 45221.2449409
cgs_7 9 113 85.9741e-15
cgd_7 9 112 17.6555e-15
 
M8 6 6 0 0 nch3 L=0.5e-6 W=8e-6
rd_8 6 114 170.0000e-3
rs_8 0 115 170.0000e-3
capbd_8 114 0 7.4799e-15
capbs_8 115 0 7.4799e-15
gm_8 114 115 6 0 284.2360e-6
rds_8 114 115 1022054.60981
cgs_8 6 115 10.8566e-15
cgd_8 6 114 2.5266e-15
 
M9 7 6 0 0 nch3 L=0.5e-6 W=8e-6
rd_9 7 116 170.0000e-3
rs_9 0 117 170.0000e-3
capbd_9 116 0 7.4799e-15
capbs_9 117 0 7.4799e-15
gm_9 116 117 6 0 295.0306e-6
rds_9 116 117 1274764.85687
cgs_9 6 117 10.9092e-15
cgd_9 6 116 2.5261e-15
 
M10 5 8 0 0 pch3 L=0.5e-6 W=80e-6
rd_10 5 118 18.0000e-3
rs_10 0 119 18.0000e-3
capbd_10 118 0 69.9237e-15
capbs_10 119 0 69.9237e-15
gm_10 118 119 8 0 574.6438e-6
rds_10 118 119 433012.903785
cgs_10 8 119 121.8909e-15
cgd_10 8 118 22.1475e-15
 

 
* the third CS stage   
 
M11 15 4 0 0 pch3 L=0.5e-6 W=80e-6
rd_11 15 120 18.0000e-3
rs_11 0 121 18.0000e-3
capbd_11 120 0 69.9237e-15
capbs_11 121 0 69.9237e-15
gm_11 120 121 4 0 3.4662e-3
rds_11 120 121 28002.1841704
cgs_11 4 121 150.1205e-15
cgd_11 4 120 22.1456e-15
 
M12 15 7 0 0 nch3 L=0.5e-6 W=6.5e-6
rd_12 15 122 209.2308e-3
rs_12 0 123 209.2308e-3
capbd_12 122 0 6.1150e-15
capbs_12 123 0 6.1150e-15
gm_12 122 123 7 0 1.3692e-3
rds_12 122 123 34346.4387895
cgs_12 7 123 11.7976e-15
cgd_12 7 122 2.0581e-15
 

 
* bais current branches
 
M13 1 1 0 0 nch3 L=0.5e-6 W=2e-6
rd_13 1 124 680.0000e-3
rs_13 0 125 680.0000e-3
capbd_13 124 0 2.0204e-15
capbs_13 125 0 2.0204e-15
gm_13 124 125 1 0 73.1612e-6
rds_13 124 125 3736599.61961
cgs_13 1 125 2.7417e-15
cgd_13 1 124 6.244559e-16
 
M14 8 8 0 0 pch3 L=0.5e-6 W=10e-6
rd_14 8 126 144.0000e-3
rs_14 0 127 144.0000e-3
capbd_14 126 0 8.8966e-15
capbs_14 127 0 8.8966e-15
gm_14 126 127 8 0 72.2754e-6
rds_14 126 127 4371072.59128
cgs_14 8 127 15.1807e-15
cgd_14 8 126 2.7575e-15
 
M15 9 9 0 0 pch3 L=0.5e-6 W=1.15e-6
rd_15 9 128 1.2522
rs_15 0 129 1.2522
capbd_15 128 0 1.1811e-15
capbs_15 129 0 1.1811e-15
gm_15 128 129 9 0 47.3018e-6
rds_15 128 129 1774228.37477
cgs_15 9 129 2.0958e-15
cgd_15 9 128 3.074756e-16
 

 
* compensation capacitors 
 
Cc1 4 10 5e-12
 
Cc2 7 11 20e-12
 
Rc 10 15 600
 
Rc2 11 10 5
 

 
* load capacitor 
 
Cl 15 0 30e-12
 
* Rl 15 0 8
 

 

 
 
 
 
 
 
*C2 12 0 1e+9
 
*L1 12 15 1e+9
 

 

VIN 13 0 ac 1
 
*.options list node post=2 unwrape-12 probe
*.option post
*.option probe
 
.op
.options list node post=2 probe unwrap 
.ac dec 10 1 1e+9
 
*.probe ac v(15)
.print ac VDB(15) VP(15) 
 

 

 
.END
 

 
