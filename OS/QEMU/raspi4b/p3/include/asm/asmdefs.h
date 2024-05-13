#ifndef __ASMDEFS_H_
#define __ASMDEFS_H__

#define ASM_NL           ;
#define STT_FUNC 2

#define ENTRY_ALIGN(name, alignment)    \
  .global name;         \
  .type name,%function; \
  .align alignment;    \
   name:                 \
  .cfi_startproc;

#define ENTRY(name)     ENTRY_ALIGN(name, 6)

#define END(name)       \
  .cfi_endproc;         \
  .size name, .-name;

#define ENDPROC(name) \
        .type name STT_FUNC ASM_NL \
        END(name)
#endif

#define SYMBOL_NAME_LABEL(X)    X:
#define SYMBOL_NAME(X)          X

#define __ALIGN .align          4
#define ALIGN                   __ALIGN

#define LENTRY(name) \
        ALIGN ASM_NL \
        SYMBOL_NAME_LABEL(name)

#define WEAK(name) \
        .weak SYMBOL_NAME(name) ASM_NL \
        LENTRY(name) \
       .cfi_startproc;

