#include <utils.h>
#include <printk.h>
#include <raspi4_types.h>
#include <debug.h>


typedef struct{
    uint64_t addr;
    char name[56];
}sym_table_t;

void getOffset(uint64_t addr){
    sym_table_t *ptr = (sym_table_t *)&ksyms;
    sym_table_t *p_ptr = NULL;
    int flag=0;
    while(ptr->name[0] != 0x0){
        if(addr < ptr->addr){
            flag=1;
            break;
        }
        p_ptr = ptr;
        ++ptr;
    }
    if(flag){
        printk("addr=0x%p is %s+0x%x\n",addr,p_ptr->name, addr-p_ptr->addr);
    }else{
        printk("addr=0x%p Unknown\n",addr);
    }
}

void display_syms(){
    sym_table_t *ptr = (sym_table_t *)&ksyms;
    while(ptr->name[0] != 0x0){
        printk("addr=0x%x, sym=%s\n",ptr->addr, ptr->name);
        ++ptr;
    }
}

void debug_init(){
    printk("kysms=%p\n",&ksyms);
    display_syms();
    getOffset(0x82A80);
}

void dump_regs(){
        unsigned int el;
        u64 val1,val2;
        el = current_el();
        if (el == 1){
               asm volatile("mrs %0, far_el1" : "=r" (val1) : : "cc");
               asm volatile("mrs %0, spsr_el1" : "=r" (val2) : : "cc");
              printk("far_el1 : 0x%x, spsr_el1 : 0x%x,\n",val1,val2);
        }else if (el == 2){
               asm volatile("mrs %0, sctlr_el1" : "=r" (val1) : : "cc");
        }else{
               asm volatile("mrs %0, sctlr_el1" : "=r" (val1) : : "cc");
        }
}
