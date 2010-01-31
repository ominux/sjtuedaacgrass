Sallen-Key topology

.param
+ rand1=gauss(10e3, 0.1, 333.3)
+ rand2=gauss(10e3, 0.1, 1666.7)

.param
+ r1=10k
+ r2=9.53k
+ r3=8.25k
+ c=0.01u
+ rf1=402
+ rf2=3.65k
+ rf3=10.2k
+ rl=10k

vin 1 0 ac 1

* stage 1
R11 1 2 r1
R12 2 4 r1
R13 3 5 rf1
R14 5 0 rl
C11 2 3 c
C12 4 0 c
*E1 3 0 OPAMP 4 5
E1 3 0 4 5 1e10

* stage 2
R21 3 6 r2
R22 6 7 r2
R23 8 9 rf2
R24 9 0 rl
C21 6 8 c
C22 7 0 c
*E2 8 0 OPAMP 7 9
E2 8 0 7 9 1e10

* stage 3
R31 8 10 r3
R32 10 11 r3
R33 12 13 rf3
R34 13 0 rl
C31 10 12 c
C32 11 0 c
*E3 12 0 OPAMP 11 13
E3 12 0 11 13 1e10

.ac dec 10 1 10meg
*.ac dec 10 1 10meg sweep monte=100
.option post unwrap probe
*.op
.print ac vdb(3) vdb(8) vdb(12)
.print ac vp(3) vp(8) vp(12)
.MEAS AC BandWidth WHEN vdb(12)=-3 FALL=LAST
*.MEAS AC PhaseMargin WHEN TRIG vdb(12) VAL=-3 FALL=LAST
*.MEAS R1_ PARAM=r1 
.end
