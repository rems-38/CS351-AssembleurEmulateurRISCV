addi a1, zero, 20
addi t2, a1, 72
beq a1, t2, -2048
sub s3, t2, a1
add s3, t2, s3
ld t0, 16(t6)
sd a5, 1244(s10)

# EXPECTED
# sp: 16384
# a1: 20
# t2: 92
# s3: 164


