start:
    addi    x6,  x0, 10

    addi    x5,  x0, 0
    addi    x31, x0, 1

    beq     x6,  x0, done_zero
    addi    x7,  x0, 1
    beq     x6,  x7, done_one

    addi    x4,  x0, 1

loop:
    blt     x6,  x4, after_loop
    add     x28, x5, x31
    add     x5,  x31, x0
    add     x31, x28, x0
    addi    x4,  x4, 1
    blt     x4,  x6, loop

after_loop:
    add     x10, x31, x0
    jal     x0, hang

done_zero:
    add     x10, x0, x0
    jal     x0, hang

done_one:
    add     x10, x31, x0
    jal     x0, hang
hang: