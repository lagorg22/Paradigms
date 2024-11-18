student_score:
    addi sp, sp, -12

    addi x10, x0, 0
    sw x10, 8(sp)

    #course adress
    lw x11, 12(sp)

    #student*
    lw x12, 8(x11)

    lw x13, 16(sp)
    mul x13, x13, 14

    #student
    lw x14, x13(x12)




    blt