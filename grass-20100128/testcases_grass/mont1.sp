mont1.sp test of monte carlo   gaussian, uniform, and limit functions

.option post  

.dc monte=60

* setup plots

.probe aunif_1=v(au1)
.probe aunif_10=v(au10)
.probe agauss_1=v(ag1)
.probe agauss_10=v(ag10)
.probe limit=v(l1)

* uniform distribution relative variation +/- .2
.param ru_1=unif(100,.2)

iu1 u1 0 -1
ru1 u1 0 ru_1

* absolute uniform distribution absolute variation +/- 20
* single throw and 10 throw maximum
.param rau_1=aunif(100,20)
.param rau_10=aunif(100,20,10)

iau1 au1 0 -1
rau1 au1 0 rau_1

iau10 au10 0 -1
rau10 au10 0 rau_10

* gaussian distribution relative variation +/- .2 at 3 sigma
.param rg_1=gauss(100,.2,3)

ig1 g1 0 -1
rg1 g1 0 rg_1

* absolute gaussian distribution absolute variation +/- .2 at 3 sigma
* single throw and 10 throw maximum
.param rag_1=agauss(100,20,3)
.param rag_10=agauss(100,20,3,10)

iag1 ag1 0 -1
rag1 ag1 0 rag_1

iag10 ag10 0 -1
rag10 ag10 0 rag_10

* random limit distribution absolute variation +/- 20
.param rl=limit(100,20)

il1 l1 0 -1
rl1 l1 0 rl
.end
