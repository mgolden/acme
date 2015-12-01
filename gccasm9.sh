gcc -S -fverbose-asm -g -O9 $1.c
as -alhnd $1.s > ${1}9.lst
