li t1, 8
mv t2, t1
li a0, 0
li s1, 5

# loop:
beq t2, zero, 12
add a0, a0, s1
addi t2, t2, -1
j -12

# end:
mv a1, a0
sub a1, a1, t1

# EXPECTED
# sp: 16384
# t1: 8
# t2: 0
# s1: 5
# a0: 40
# a1: 32