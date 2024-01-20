li a7, 50
li s1, 25

sub a7, a7, s1
beq a7, s1, 4
add a7, s1, zero

# jump du beq
addi s1, zero

# EXPECTED
# sp : 16384
# a7 : 25
# s1 : 0