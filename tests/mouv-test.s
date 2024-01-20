addi t6, zero, 21
addi a6, zero 36

bge a6, t6, 0
sub a6, a6, t6
mv t4, a6

# EXPECTED
# sp : 16384
# t4 : 15
# t6 : 21
# a6 : 15