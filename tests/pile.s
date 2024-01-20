addi s0, zero, 20

addi sp, sp, -16
sd s0, 8(sp)

addi s0, s0, -5

ld s1, 8(sp)

# EXPECTED
# sp: 16376
# s0: 15
# s1: 20 