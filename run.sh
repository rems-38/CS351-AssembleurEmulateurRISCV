#!/bin/sh

if [ $1 ] && [ "./tests/$1.s" ] ; then
    make

    echo "Running assembler..."
    ./riscv-assembler ./tests/$1.s ./tests/$1.hex
    echo "$1 assembled successfully!"
    echo
    echo "Running emulator..."
    ./riscv-emulator ./tests/$1.hex ./tests/$1.state
    echo "$1 emulated successfully!"
    echo
    echo "Result :"
    cat ./tests/$1.state
fi