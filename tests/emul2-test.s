addi t2, zero, 60
addi s3, zero, -10
li a1, 10 

bne t2, s3, 8
mv a1, s3

#jump du bne
add s3, s3, a1

# end
add zero, zero, t2

# EXPECTED
# sp : 16384
# t2 : 60
# s3 : 0
# a1 : 10
