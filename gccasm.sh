gcc -S -fverbose-asm -g -O0 $1.c
as -alhnd $1.s > $1.lst
