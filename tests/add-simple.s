addi a1, zero, 20
addi a2, zero, 22
add a0, a1, a2

# ld 
# sd

sub t1, t0, t1
# 40628333

beq a5, t1, 8
# 00678463

bne ra, zero, 4
# 00009263

blt gp, s4, 4
# 0141c263

bge sp, s7, 16
# 01715863

jal zero, 256
# 1000006f

# EXPECTED
# sp: 16384
# a0: 42
# a1: 20
# a2: 22
