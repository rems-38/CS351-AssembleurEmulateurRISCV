addi a1, zero, 20
jal s4, 8
add a1, a1, a1
addi a1, a1, 2

# EXPECTED
# sp: 16384
# a1: 22
# s4: 8