main:
    addi sp, sp, -12
    sw ra, 8(sp)
    li x10, 2
    li x11, 3
    sw x10, 0(sp)
    sw x11, 4(sp)

    jal process

    lw x10, 0(sp)

    lw ra, 8(sp)
    addi sp, sp, 12
    ret

process:
    addi sp, sp, -8
    sw ra, 4(sp)

    jal power_four
    lw x10, 4(sp)
    lw x11, 8(sp)
    mul x11, x11, x11
    add x10, x11
    sw x10, 4(sp)

    lw ra, 4(sp)
    addi sp, sp, 4
    ret

power_four:
    lw x10, 4(sp)
    mul x10, x10, x10
    mul x10, x10, x10
    sw x10, 4(sp)
    ret