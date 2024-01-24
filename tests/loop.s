li a0, 42
li a1, 86
addi a1, a1, -42
bge a1, a0, -4

# EXPECTED
# sp: 16384
# a0: 42
# a1: 2