.section .text.boot
.global _start

_start:
    mrs	    x1, mpidr_el1
    and	    x1,x1, #3
    cbz	    x0, 2f
1:  // cpu_id > 0, do loop
    b	    1b
2:  // cpu_id == 0 
    // set top of stack just before our code (stack grows to a lower address per AAPCS64)
    ldr	    x1, =_start
    mov	    sp, x1
    // clear bss
    ldr	    x1, =__bss_start
    ldr	    w2, =__bss_size
    cbz	    w2, 4f
3:  str	    xzr, [x1], #8
    sub	    w2, w2, #1
    cbnz    w2, 3b
4:  bl	    main
    b	    1b
