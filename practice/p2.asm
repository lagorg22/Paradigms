
main:
    addi sp, sp, -4
    sw ra, 0(sp)
    li x10, 2
    li x11, 3

    jal process


    li x10, 0

    lw ra, 0(sp)
    addi sp, sp, 4
    ret

process:
    addi sp, sp, -4
    sw ra, 0(sp)

    jal power_four

    mul x11, x11, x11
    add x10, x10, x11


    lw ra, 0(sp)
    addi sp, sp, 4
    ret

power_four:
    mul x10, x10, x10
    mul x10, x10, x10
    ret



main:
    addi sp, sp, -4       # Allocate stack space
    sw ra, 0(sp)          # Save return address
    li x10, 2             # value1 = 2
    li x11, 3             # value2 = 3

    jal process           # Call process(2, 3)

    li x10, 0             # Return value = 0

    lw ra, 0(sp)          # Restore return address
    addi sp, sp, 4        # Deallocate stack space
    ret

process:
    addi sp, sp, -8       # Allocate stack space for ra and x11
    sw ra, 4(sp)          # Save return address
    sw x11, 0(sp)         # Save y (x11) because power_four might modify it

    jal power_four        # Call power_four(x)
                         # x10 now contains x^4

    lw x11, 0(sp)         # Restore original y value
    mul x11, x11, x11     # Calculate y^2
    add x10, x10, x11     # Add x^4 + y^2

    lw ra, 4(sp)          # Restore return address
    addi sp, sp, 8        # Deallocate stack space
    ret

power_four:
    mul x10, x10, x10     # x^2
    mul x10, x10, x10     # x^4
    ret