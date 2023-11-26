add a0, a1, a2
sub t1, t0, t1
addi a1, zero, 20
ld t0, 8(t6)
sd a5, 8(s10)
beq a5, t1, 8
bne ra, zero, 4
blt gp, s4, 4
bge sp, s7, 16
jal zero, 256
j 128
li t4, 25
mv t3, a0


# EXPECTED (assembling part only)
# 00c58533
# 40628333
# 01400593
# 008fb283
# 00fd3423
# 00678463
# 00009263
# 0141c263
# 01715863
# 1000006f
# 0800006f
# 01900e93
# 00050e13
