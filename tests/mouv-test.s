addi t6, zero, 21
addi a6, zero, 36

bge t6, a6, 4
mv t4, a6
sub a6, a6, t6

# EXPECTED
# sp : 16384
# t4 : 36
# t6 : 21
# a6 : 15