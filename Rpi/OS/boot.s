.section .text
.global start

start:
    mrs x0, mpidr_el1
    and x0,x0, #3
    cmp x9, #0
    beq kernel_entry

end:
    b end



kernel_entry:
    ldr x1, =start
    mov sp, x1
    bl KMain
    b end
