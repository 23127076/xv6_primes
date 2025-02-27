// before run the below, copy the primes into /xv6-riscv/user
// type in Terminal
//bash
cd /home/c-lane/xv6-riscv
//bash
make clean
//bash
cd /home/c-lane/xv6-riscv
//bash
nano Makefile
//Makefile
/*
add:
UPROGS=\
    _cat\
    _echo\
    _grep\
    _init\
    _ls\
    _primes\     # Add _primes here or // $U/_primes\
    ...
*/
//bash
make qemu
//bash
primes
