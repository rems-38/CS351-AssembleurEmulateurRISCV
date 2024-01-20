li s0, 20

addi sp, sp, -16
sd s0, 8(sp)

addi s0, s0, -5

ld s1, 8(sp)

# EXPECTED
# sp: 16368
# s0: 15
# s1: 20