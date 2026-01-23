adda.w #$20, a0|D0FC 0020
adda.w ($FFFFF100).w, a2|D4F8 F100
adda.l ($FFFFF100).l, a3|D7F9 FFFF F100
adda.w d0, a2|D4C0
adda.w (a0), a1|D2D0
adda.w $10(a0), a1|D2E8 0010
adda.w (a0)+, a1|D2D8
adda.w -(a0), a1|D2E0
adda.w #$1337, a4|D8FC 1337
adda.w (a3,d1.w), a3|D6F3 1000
adda.w $24(a3,d4.w), a3|D6F3 4024
addi.b #$20, d1|0601 0020
addi.l #$FFFFE6AC, d0|0680 FFFF E6AC
addi.w #$1337, ($FFFFF100).w|0678 1337 F100
addi.w #$1337, ($FFFFF100).l|0679 1337 FFFF F100
addi.l #$19650917, ($FFFFF100).w|06B8 1965 0917 F100
addi.b #$40, (a1)|0611 0040
addi.w #$40, $1C(a1)|0669 0040 001C
addi.l #$500, $64(a1)|06A9 0000 0500 0064
addi.b #$40, (a1)+|0619 0040
addi.b #$40, -(a1)|0621 0040
addq.b #1, ($FFFFF100).w|5238 F100
addq.b #2, ($FFFFF100).w|5438 F100
addq.b #8, ($FFFFF100).w|5038 F100
addq.b #1, d6|5206
addq.w #3, d2|5642
addq.w #8, (a1)|5051
addq.b #1, $25(a0)|5228 0025
addq.w #1, $14(a0)|5268 0014
addq.w #8, (a1)+|5059
addq.w #8, -(a1)|5061
addq.w #1, a0|5248
addq.l #1, a0|5288
addq.l #4, sp|588F
add.w d2, d0|D042
add.w d6, d2|D446
add.w ($FFFFEE8C).w, d0|D078 EE8C
add.w (a1), d0|D051
add.b 1(a3), d1|D22B 0001
add.w $46(a0), d2|D468 0046
add.w (a1)+, d0|D059
add.w -(a1), d0|D061
add.l a0, d1|D288
add.w d1, ($FFFFFFAC).w|D378 FFAC
add.w d0, (a1)|D151
add.w d1, $1A(a0)|D368 001A
add.l d0, $14(a0)|D1A8 0014
add.l d3, (a5)+|D79D
add.w d3, -(a1)|D761
andi.b #$36, d1|0201 0036
andi.w #$100, d2|0242 0100
andi.l #$FFFFFF, d0|0280 00FF FFFF
andi.b #$70, ($FFFFF100).w|0238 0070 F100
andi.w #$2005, ($FFFFF100).w|0278 2005 F100
andi.b #$80, (a1)|0211 0080
andi.b #$80, $2B(a1)|0229 0080 002B
andi.b #$80, (a1)+|0219 0080
andi.b #$80, -(a1)|0221 0080
andi.w #$777, 4(a4)|026C 0777 0004
andi.b #$7F, (a2,d0.w)|0232 007F 0000
andi.w #$7FF, $10(a2,d4.w)|0272 07FF 4010
andi #$FFFE,sr|027C FFFE
and.b d0, d1|C200
and.b ($FFFFF607).w, d0|C038 F607
and.w ($FFFFEEAC).w, d3|C678 EEAC
and.w (a0), d0|C050
and.w 6(a5), d4|C86D 0006
and.w $A(a5), d5|CA6D 000A
and.w (a0)+, d0|C058
and.w -(a0), d0|C060
and.w d0, ($FFFFF606).w|C178 F606
and.w d0, (a0)|C150
and.w d0, $14(a0)|C168 0014
and.w d1, $A(a0)|C368 000A
and.w d0, (a0)+|C158
and.w d0, -(a0)|C160
bra.s #$24|6024
bra.w #$4E1A|6000 4E1A
bsr.s #$10|6110
bhi.s #$10|6210
bls.s #$10|6310
bcc.s #$10|6410
bcs.s #$10|6510
bne.s #$10|6610
beq.s #$10|6710
bvc.s #$10|6810
bvs.s #$10|6910
bpl.s #$10|6A10
bmi.s #$10|6B10
bge.s #$10|6C10
blt.s #$10|6D10
bgt.s #$10|6E10
ble.s #$10|6F10
btst #6, ($FFFFF604).w|0838 0006 F604
btst #4, d1|0801 0004
btst #6, (a0)|0810 0006
btst #6, $2A(a0)|0828 0006 002A
btst d6, ($FFFFF604).w|0D38 F604
btst d6, d1|0D01
btst d6, (a0)|0D10
btst d6, $2A(a0)|0D28 002A
bclr #7, ($FFFFE43D).w|08B8 0007 E43D
bclr #7, d2|0882 0007
bclr #3, (a1)|0891 0003
bclr #3, $2A(a1)|08A9 0003 002A
bclr d6, ($FFFFE43D).w|0DB8 E43D
bclr d6, d2|0D82
bclr d6, (a0)|0D90
bclr d6, $2A(a0)|0DA8 002A
bset #3, ($FFFFFE05).w|08F8 0003 FE05
bset #3, d2|08C2 0003
bset #2, (a0)|08D0 0002
bset #2, $2A(a0)|08E8 0002 002A
bset d0, ($FFFFFE05).w|01F8 FE05
bset d0, d2|01C2
bset d0, (a2)|01D2
bset d0, $2A(a2)|01EA 002A
bchg #4, ($FFFFFFB2).w|0878 0004 FFB2
bchg #4, d2|0842 0004
bchg #4, (a3)|0853 0004
bchg #4, $2A(a3)|086B 0004 002A
bchg d3, ($FFFFFFB2).w|0778 FFB2
bchg d3, d2|0742
bchg d3, (a3)|0753
bchg d3, $2A(a3)|076B 002A
clr.w d2|4242
clr.l d0|4280
clr.w ($FFFF8500).w|4278 8500
clr.w ($FFFF4500).l|4279 FFFF 4500
clr.b $3C(a0)|4228 003C
clr.w $1A(a0)|4268 001A
clr.l (a1)|4291
clr.l 4(a1)|42A9 0004
clr.l (a1)+|4299
clr.l -(a1)|42A1
clr.l (sp)+|429F
cmpa.w #$B000, a1|B2FC B000
cmpa.l #$FFFFB000, a5|BBFC FFFF B000
cmpa.l ($FFFFE660).w, a2|B5F8 E660
cmpa.w d0, a2|B4C0
cmpa.l (a1), a2|B5D1
cmpa.l 8(a1), a2|B5E9 0008
cmpa.l a1, a2|B5C9
cmpi.b #$26, ($FFFFFFE4).w|0C38 0026 FFE4
cmpi.w #$4170, ($FFFFFFB0).w|0C78 4170 FFB0
cmpi.l #$FFFE8000, ($FFFFEE9C).w|0CB8 FFFE 8000 EE9C
cmpi.w #$3, ($FFFFFF08).l|0C79 0003 FFFF FF08
cmpi.b #$2, d0|0C00 0002
cmpi.w #$2E0, d3|0C43 02E0
cmpi.l #$0, d2|0C82 0000 0000
cmpi.b #$2, (a0)|0C10 0002
cmpi.b #$2, $20(a1)|0C29 0002 0020
cmpi.w #$1044, $10(a0)|0C68 1044 0010
cmpi.b #$2, (a0)+|0C18 0002
cmpi.b #$2, -(a0)|0C20 0002
cmp.b d0, d1|B200
cmp.l d0, d7|BE80
cmp.w ($FFFFEE78).w, d0|B078 EE78
cmp.l ($FFFFEE98).w, d6|BCB8 EE98
cmp.w (a0), d0|B050
cmp.b 1(a3), d0|B02B 0001
cmp.w $14(a0), d0|B068 0014
cmp.w (a6)+, d0|B05E
cmp.b -(a2), d0|B022
dbf d0, $556A|51C8 556A
dbf d3, $FFA0|51CB FFA0
dbeq d1, $FFFC|57C9 FFFC
dbmi d4, $FFDC|5BCC FFDC
divs.w #$A, d0|81FC 000A
divu.w ($FFFFF314).w, d1|82F8 F314
divu.w d1, d1|82C1
divu.w (a1), d1|82D1
divu.w 4(a0), d1|82E8 0004
divu.w (a1)+, d1|82D9
divu.w -(a1), d1|82E1
eori.w #$8810, d7|0A47 8810
eori.b #$80, ($FFFFF00E).w|0A38 0080 F00E
eori.w #$3119, ($FFFFF010).w|0A78 3119 F010
eori.l #$10001000, d5|0A85 1000 1000
eori.l #$FFFF, (a0)|0A90 0000 FFFF
eori.l #$FFFF, $40(a0)|0AA8 0000 FFFF 0040
eori.l #$FFFF, (a0)+|0A98 0000 FFFF
eori.l #$FFFF, -(a0)|0AA0 0000 FFFF
eori #$1, sr|0A7C 0001
eor.b d0, d1|B101
eor.w d5, d7|BB47
exg d1, d2|C342
exg a3, a4|C74C
exg d5, a6|CB8E
ext.w d0|4880
ext.l d1|48C1
illegal|4AFC
jmp (a1)|4ED1
jmp $10(a1)|4EE9 0010
jmp $65A70|4EF9 0006 5A70
jsr $4B98C|4EB9 0004 B98C
lea ($FFFFFFF4).w, a0|41F8 FFF4
lea ($FFFF7CC0).l, a1|43F9 FFFF 7CC0
lea d3, a2|45C3
lea (a6), a2|45D6
lea $130(a1), a1|43E9 0130
lea (a3,d3.w), a2|45F3 3000
lea 8(a3,d3.w), a2|45F3 3008
lea $4C(pc,d0.w), a1|43FB 004C
link a4, #$1087|4E54 1087
lsl.b #1, d1|E309
lsr.w #7, d1|EE49
lsr.w #8, d1|E049
lsr.w d2, d2|E46A
lsl.l #8, d0|E188
asr.l #3, d0|E680
asr.l #8, d1|E081
asr $1C(a1)|E0E9 001C
move.b #$64, ($FFFFFFE0).w|11FC 0064 FFE0
move.w #$03E8, ($FFFFFFE0).w|31FC 03E8 FFE0
move.l #$05F5E100, ($FFFFFFE0).w|21FC 05F5 E100 FFE0
move.b #$20, d1|123C 0020
move.l #$60000003, d0|203C 6000 0003
move.w #$101, (a1)|32BC 0101
move.b #$3C, 2(a2)|157C 003C 0002
move.w #$2FA0, $10(a1)|337C 2FA0 0010
move.w #$0, (a3)+|36FC 0000
move.w #$0, -(a3)|373C 0000
move.w ($FFFFFF0A).w, ($FFFFFF08).w|31F8 FF0A FF08
move.w ($FFFFFF0A).w, ($FFFFFF08).l|33F8 FF0A FFFF FF08
move.w ($FFFF01A5).l, ($FFFFFFE0).w|31F9 FFFF 01A5 FFE0
move.w ($FFFFEE18).w, d0|3038 EE18
move.b ($FFFFFF0B).w, (a0)|10B8 FF0B
move.b ($FFFFFF0B).w, $22(a0)|1178 FF0B 0022
move.b ($FFFFFF0B).w, (a0)+|10F8 FF0B
move.b ($FFFFFF0B).w, -(a0)|1138 FF0B
move.w ($FFFFFF38).w, -(sp)|3F38 FF38
move.l ($FFFFFF3A).w, -(sp)|2F38 FF3A
move.b d0, ($FFFFF604).w|11C0 F604
move.l d1, ($FFFFFFE0).l|23C1 FFFF FFE0
move.w d1, d3|3601
move.l d0, d7|2E00
move.w d0, (a6)|3C80
move.b d3, $26(a0)|1143 0026
move.w d3, (a2)+|34C3
move.w d3, -(a2)|3503
move.b (a3), d1|1213
move.b (a3), (a4)|1893
move.b (a1), 3(a2)|1551 0003
move.l (a0), $80(a2)|2550 0080
move.b (a3), (a4)+|18D3
move.b (a3), -(a4)|1913
move.w ($34(a0)), ($FFFFFF08).w|31E8 0034 FF08
move.b 8(a1), d0|1029 0008
move.b $26(a0), d6|1C28 0026
move.l $64(a1), d3|2629 0064
move.b 1(a2), (a2)|14AA 0001
move.b 3(a2), 2(a2)|156A 0003 0002
move.w $14(a1), $54(a2)|3569 0014 0054
move.l $10(a1), $50(a2)|2569 0010 0050
move.b 1(a2), (a2)+|14EA 0001
move.b 1(a2), -(a2)|152A 0001
move.b (a4)+, ($FFFFFFB2).w|11DC FFB2
move.b (a4)+, d0|101C
move.l (a4)+, d0|201C
move.l (a0)+, (a1)|2298
move.l (a1)+, $74(a2)|2559 0074
move.l (a0)+, (a1)+|22D8
move.b -(a3), d0|1023
move.w -(a4), d0|3024
move.w -(a5), -(a1)|3325
move.l a1, ($FFFFE446).w|21C9 E446
move.l a1, d0|2009
move.l a1, (a0)|2089
move.w a2, $3C(a0)|314A 003C
move.l a2, (sp)|2E8A
move.l a2, (sp)+|2ECA
move.l a2, -(sp)|2F0A
move.b $14(a2,d6.w), ($FFFFE446).w|11F2 6014 E446
move.b (a2,d0.w), d1|1232 0000
move.w 4(a4,d0.w), d6|3C34 0004
move.b $1D(a3,d4.w), d4|1833 401D
move.b (a2,d2.w), (a3)|16B2 2000
move.b $14(a2,d6.w), $3C(a3)|1772 6014 003C
move.b (a2,d2.w), (a3)+|16F2 2000
move.b (a2,d2.w), -(a3)|1732 2000
move.b $5E(pc,d3.w), ($FFFFE446).w|11FB 305E E446
move.b $5E(pc,d0.w), d2|143B 005E
move.b $5E(pc,d0.w), (a1)|12BB 005E
move.b $5E(pc,d0.w), $22(a1)|137B 005E 0022
move #$2700,sr|46FC 2700
move d6,ccr|44C6
move sr,($FFFFFE26).w|40F8 FE26
move sr,d6|40C6
move sr,(a2)|40D2
move sr,4(a2)|40EA 0004
move sr,(a2)+|40DA
move sr,-(a2)|40E2
move sr,4(sp)|40EF 0004
move sr,-(sp)|40E7
movea.w #$6000, a4|387C 6000
movea.w ($FFFFEE4A).w, a4|3878 EE4A
movea.w d1, a4|3841
movea.l a2, a6|2C4A
movea.w (a3), a1|3253
movea.w $2C(a3), a1|326B 002C
movea.l (a1)+, a2|2459
movea.l -(a1), a2|2461
movea.l (sp), a2|2457
movea.l (sp)+, a2|245F
movea.l -(sp), a2|2467
movea.l (a4,d0.w), a3|2674 0000
movea.l $18(a4,d0.w), a3|2674 0018
movem.l d0-d4/a0-a2, ($FFFFFF10).w|48F8 071F FF10
movem.l d0-d5/a0-a2, ($FFFFFF10).w|48F8 073F FF10
movem.l d0-d6/a0-a2, ($FFFFFF10).w|48F8 077F FF10
movem.l d0-d6/a0-a4, ($FFFFFF10).w|48F8 1F7F FF10
movem.l ($FFFFFF10).w, d0-d4/a0-a2|4CF8 071F FF10
movem.l ($FFFFFF10).w, d0-d5/a0-a2|4CF8 073F FF10
movem.l ($FFFFFF10).w, d0-d6/a0-a2|4CF8 077F FF10
movem.l ($FFFFFF10).w, d0-d6/a0-a4|4CF8 1F7F FF10
movem.w d0-d1, -(sp)|48A7 C000
movem.w d0-d2, -(sp)|48A7 E000
movem.l a0-a3, -(a6)|48E6 00F0
movem.l a0-a5, -(a6)|48E6 00FC
movem.l d0-d1, -(sp)|48E7 C000
movem.l d0-d5, -(sp)|48E7 FC00
movem.l d0-a6, -(sp)|48E7 FFFE
movem.w (sp)+, d0-d1|4C9F 0003
movem.w (sp)+, d0-d2|4C9F 0007
movem.l (sp)+, d0-d5|4CDF 003F
movem.l (sp)+, d1-d2|4CDF 0006
movem.l (sp)+, d1-d4|4CDF 001E
movem.l (sp)+, a2-a4|4CDF 1C00
movem.l (sp)+, a4-a6|4CDF 7000
movem.l (sp)+, d0-a6|4CDF 7FFF
movep.w 0(a0), d3|0708 0000
movep.l 0(a0), d3|0748 0000
movep.w d2, 0(a0)|0588 0000
movep.l d2, 0(a0)|05C8 0000
moveq #$80, d1|7280
muls.w #$19, d1|C3FC 0019
muls.w ($FFFFFF9E).w, d1|C3F8 FF9E
muls.w d2, d1|C3C2
muls.w (a0), d1|C3D0
muls.w $1C(a0), d1|C3E8 001C
muls.w (a0)+, d1|C3D8
muls.w -(a0), d1|C3E0
mulu.w #$19, d2|C4FC 0019
mulu.w ($FFFFFF9E).w, d2|C4F8 FF9E
mulu.w d2, d1|C2C2
mulu.w (a3), d2|C4D3
mulu.w $1C(a3), d2|C4EB 001C
mulu.w (a3)+, d2|C4DB
mulu.w -(a3), d2|C4E3
neg.w ($FFFFFE26).w|4478 FE26
neg.l d0|4480
neg.w (a1)|4451
neg.w $1A(a1)|4469 001A
neg.w (a1)+|4459
neg.w -(a1)|4461
nop|4E71
not.w ($FFFFFE26).w|4678 FE26
not.l d0|4680
not.w (a1)|4651
not.w $1A(a1)|4669 001A
not.w (a1)+|4659
not.w -(a1)|4661
ori.b #$36, d1|0001 0036
ori.w #$100, d2|0042 0100
ori.l #$FFFFFF, d0|0080 00FF FFFF
ori.b #$70, ($FFFFF100).w|0038 0070 F100
ori.w #$2005, ($FFFFF100).w|0078 2005 F100
ori.b #$80, (a1)|0011 0080
ori.b #$80, $2B(a1)|0029 0080 002B
ori.b #$80, (a1)+|0019 0080
ori.b #$80, -(a1)|0021 0080
ori.w #$777, 4(a4)|006C 0777 0004
ori.b #$7F, (a2,d0.w)|0032 007F 0000
ori.w #$7FF, $10(a2,d4.w)|0072 07FF 4010
ori #$0001,sr|007C 0001
or.b d0, d1|8200
or.b ($FFFFF607).w, d0|8038 F607
or.w ($FFFFEEAC).w, d3|8678 EEAC
or.w (a0), d0|8050
or.w 6(a5), d4|886D 0006
or.w $A(a5), d5|8A6D 000A
or.w (a0)+, d0|8058
or.w -(a0), d0|8060
or.w d0, ($FFFFF606).w|8178 F606
or.w d0, (a0)|8150
or.w d0, $14(a0)|8168 0014
or.w d1, $A(a0)|8368 000A
or.w d0, (a0)+|8158
or.w d0, -(a0)|8160
pea ($FFFFE53C).w|4878 E53C
pea (a2)|4852
pea $2A(a0)|4868 002A
rol.w d0, d1|E179
ror.b #1, d0|E218
ror.l #1, d3|E29B
rol.w ($FFFFFFE0).w|E7F8 FFE0
rte|4E73
rtr|4E77
rts|4E75
st ($FFFFF000).w|50F8 F000
sf ($FFFFF000).w|51F8 F000
shi ($FFFFF000).w|52F8 F000
sls ($FFFFF000).w|53F8 F000
scc ($FFFFF000).w|54F8 F000
scs ($FFFFF000).w|55F8 F000
sne ($FFFFF000).w|56F8 F000
seq ($FFFFF000).w|57F8 F000
svc ($FFFFF000).w|58F8 F000
svs ($FFFFF000).w|59F8 F000
spl ($FFFFF000).w|5AF8 F000
smi ($FFFFF000).w|5BF8 F000
sge ($FFFFF000).w|5CF8 F000
slt ($FFFFF000).w|5DF8 F000
sgt ($FFFFF000).w|5EF8 F000
sle ($FFFFF000).w|5FF8 F000
stop #$2500|4E72 2500
suba.w #$20, a0|90FC 0020
suba.w ($FFFFF100).w, a2|94F8 F100
suba.l ($FFFFF100).l, a3|97F9 FFFF F100
suba.w d0, a2|94C0
suba.w (a0), a1|92D0
suba.w $10(a0), a1|92E8 0010
suba.w (a0)+, a1|92D8
suba.w -(a0), a1|92E0
suba.w #$1337, a4|98FC 1337
suba.w (a3,d1.w), a3|96F3 1000
suba.w $24(a3,d4.w), a3|96F3 4024
subi.b #$20, d1|0401 0020
subi.l #$FFFFE6AC, d0|0480 FFFF E6AC
subi.w #$1337, ($FFFFF100).w|0478 1337 F100
subi.w #$1337, ($FFFFF100).l|0479 1337 FFFF F100
subi.l #$19650917, ($FFFFF100).w|04B8 1965 0917 F100
subi.b #$40, (a1)|0411 0040
subi.w #$40, $1C(a1)|0469 0040 001C
subi.l #$500, $64(a1)|04A9 0000 0500 0064
subi.b #$40, (a1)+|0419 0040
subi.b #$40, -(a1)|0421 0040
subq.b #1, ($FFFFF100).w|5338 F100
subq.b #2, ($FFFFF100).w|5538 F100
subq.b #8, ($FFFFF100).w|5138 F100
subq.b #1, d6|5306
subq.w #3, d2|5742
subq.w #8, (a1)|5151
subq.b #1, $25(a0)|5328 0025
subq.w #1, $14(a0)|5368 0014
subq.w #8, (a1)+|5159
subq.w #8, -(a1)|5161
subq.w #1, a0|5348
subq.l #1, a0|5388
subq.l #4, sp|598F
sub.w d2, d0|9042
sub.w d6, d2|9446
sub.w ($FFFFEE8C).w, d0|9078 EE8C
sub.w (a1), d0|9051
sub.b 1(a3), d1|922B 0001
sub.w $46(a0), d2|9468 0046
sub.w (a1)+, d0|9059
sub.w -(a1), d0|9061
sub.l a0, d1|9288
sub.w d1, ($FFFFFFAC).w|9378 FFAC
sub.w d0, (a1)|9151
sub.w d1, $1A(a0)|9368 001A
sub.l d0, $14(a0)|91A8 0014
sub.l d3, (a5)+|979D
sub.w d3, -(a1)|9761
swap d0|4840
tas.b ($FFFFFE00).w|4AF8 FE00
tas.b d1|4AC1
tas.b (a2)|4AD2
tas.b $2A(a0)|4AE8 002A
trap #1|4E41
trapv|4E76
tst.b d4|4A04
tst.l d1|4A81
tst.b ($FFFFAA80).w|4A38 AA80
tst.w ($FFFFAA80).w|4A78 AA80
tst.w ($A1000C).l|4A79 00A1 000C
tst.l ($A10008).l|4AB9 00A1 0008
tst.w (a4)|4A54
tst.w $1A(a1)|4A69 001A
unlk a0|4E58