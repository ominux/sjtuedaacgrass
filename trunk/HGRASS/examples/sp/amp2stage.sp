*Basic 2 Stage CMOS AMP Circuit
*IODEF: VIN inp 0 1 VOUT outp

.lib 'log018.l' tt_3v

*Resources Statement
Vdd vdd gnd 1.8
Vinp inp gnd dc 0.6 ac 1.0
Vinn inn gnd dc 0.6
*R1 outp inn 0
*Description of the sub-circuit
*main circuit
I1 vdd nod1 20u
I2 vdd outp 100u
*I3 vdd outn 100u

M1 nod2 inp nod1 nod1 pch3 w=7u l=1.8u
M2 nod3 inn nod1 nod1 pch3 w=7u l=1.8u
M3 nod2 nod3 gnd gnd nch3 w=3u l=1.8u
M4 nod3 nod3 gnd gnd nch3 w=3u l=1.8u
M5 outp nod2 gnd gnd nch3 w=30u l=1.8u
*M6 outn nod3 gnd gnd nch3 w=30u l=1.8u

Cp nod2 outp 2p
*Cn nod3 outn 2p

Cl outp gnd 10p
*Vb nodb gnd 0.9
.op
.options list node post=2 probe unwrap
.ac dec 1000 1 10x
*BW and PM
.print ac VDB(outp) VP(outp)
*.pz V(outp) Vin
.end