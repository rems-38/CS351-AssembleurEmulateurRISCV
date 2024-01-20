addi t4, zero, 56
addi t5, zero, 42

blt t4, t5, 12
addi a0, zero, 2
addi a1, zero, 80
j 8

# jump du blt
addi a0, zero, 1

# end
add zero, zero, zero

# EXPECTED
# sp: 16384
# a0: 2
# a1: 80
# t4: 56
# t5: 42