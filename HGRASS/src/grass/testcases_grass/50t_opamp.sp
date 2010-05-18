* Bandwidth and phase for the three-stage opamp ABNORMAL
*IODEF: VIN 1 1 VOUT 26 
.lib "log018.l" TT_3V 

* Opamp circuit ------------------------------------------------------------------
Vdd vdd gnd  1.5
Vin 1 gnd dc 1.5 ac 1.0

Vbias1 9 gnd 1.5
Vbias2 10 gnd 1.5

M1 2 2 gnd gnd nch3 L=0.5u W=2u   
M2 3 2 gnd gnd nch3 L=0.5u W=2u   
M3 8 2 gnd gnd nch3 L=0.5u W=2u    
M4 8 8 vdd vdd pch3 L=0.5u W=2u 
M5 4 4 vdd vdd pch3 L=0.5u W=2u 
M6 7 8 vdd vdd pch3 L=0.5u W=2u 
M7 27 9 7 7 pch3 L=0.5u W=2u
M8 28 10 7 7 pch3 L=0.5u W=2u
M9 12 12 gnd gnd nch3 L=0.5u W=2u    
M10 12 3 7 7 pch3 L=0.5u W=2u 
M11 7 6 vdd vdd pch3 L=0.5u W=2u 
M12 5 4 vdd vdd pch3 L=0.5u W=2u 
M13 11 12 gnd gnd nch3 L=0.5u W=2u
M14 6 1 11 11 nch3 L=0.5u W=2u
M15 6 6 vdd vdd pch3 L=0.5u W=2u
M16 16 9 11 11 nch3 L=0.5u W=2u
M17 19 10 11 11 nch3 L=0.5u W=2u
M18 5 5 gnd gnd nch3 L=0.5u W=2u
M19 11 5 gnd gnd nch3 L=0.5u W=2u
M20 14 14 gnd gnd nch3 L=0.5u W=2u
M21 13 3 14 14 pch3 L=0.5u W=2u
M22 13 4 vdd vdd pch3 L=0.5u W=2u
M23 15 14 gnd gnd nch3 L=0.5u W=2u
M24 17 13 15 15 nch3 L=0.5u W=2u
M25 18 16 15 15  nch3 L=0.5u W=2u
M26 17 18 vdd vdd pch3 L=0.5u W=2u
M27 18 18 vdd vdd pch3 L=0.5u W=2u
M28 27 20 gnd gnd nch3 L=0.5u W=2u
M29 28 20 gnd gnd nch3 L=0.5u W=2u
M30 20 1 27 27 nch3 L=0.5u W=2u
M31 21 1 28 28 nch3 L=0.5u W=2u
M32 20 3 16 16 pch3 L=0.5u W=2u
M33 21 3 19 19 pch3 L=0.5u W=2u
M34 16 17 vdd vdd pch3 L=0.5u W=2u
M35 19 17 vdd vdd pch3 L=0.5u W=2u
M36 22 22 gnd gnd nch3 L=0.5u W=2u
M37 22 21 vdd vdd pch3 L=0.5u W=2u
M38 23 22 gnd gnd nch3 L=0.5u W=2u
M39 24 22 gnd gnd nch3 L=0.5u W=2u
M40 23 23 gnd gnd nch3 L=0.5u W=2u
M41 26 23 gnd gnd nch3 L=0.5u W=2u
M42 23 3 vdd vdd pch3 L=0.5u W=2u
M43 24 24 vdd vdd pch3 L=0.5u W=2u
M44 26 24 vdd vdd pch3 L=0.5u W=2u

* compensation capacitors 
C1 21 25 20p
R1 1 2  100
R2 3 4 100
R3 25 26 100

* Load capacitor
C2 26 0 30p

* analysis----------------------------------------------
.options list node post=1 unwrapp
.op
.ac dec 10 1 1g 
.print ac VDB(26) VP(26) 

.measure ac   'unitfreq'  	trig at=1 targ vdb(26) val=0 fall=1

.measure ac   'phasemargin'  	find par('vp(26)+180') when vdb(26)=0

.measure ac   'gain(db)' 	max vdb(26)

.measure ac   'gain(mag)' 	max vm(26)

.END
