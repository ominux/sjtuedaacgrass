Sallen-Key topology: GRASS
vin 1 0 ac 1

* stage 1
r11 1 2 10k
r12 2 4 10k
r13 3 5 402
r14 5 0 10k
c11 2 3 0.01u
c12 4 0 0.01u
e1 3 0 4 5 1e10

* stage 2
r21 3 6 9.53k
r22 6 7 9.53k
r23 8 9 3.65k
r24 9 0 10k
c21 6 8 0.01u
c22 7 0 0.01u
e2 8 0 7 9 1e10

* stage 3
r31 8 10 8.25k
r32 10 11 8.25k
r33 12 13 10.2k
r34 13 0 10k
c31 10 12 0.01u
c32 11 0 0.01u
e3 12 0 11 13 1e10

.ac dec 10 1 10meg
.option post
.print ac v(12)
*.print ac vdb(3) vdb(8) vdb(12)
*.print ac vp(3) vp(8) vp(12)

.end
