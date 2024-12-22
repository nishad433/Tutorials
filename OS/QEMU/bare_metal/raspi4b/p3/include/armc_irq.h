#ifndef _ARMC_IRQ_H_
#define _ARMC_IRQ_H_

#include<base.h>
#include<raspi4_types.h>

#define TIMER_IRQ       0x1
#define CNTVIRQ_SRC_BIT 0x3

typedef struct{
        rw_REG32 ARM_CONTROL;                   // [0x00] ARM Timer and AXI Error IRQ control
        ro_REG32 reserved0[2];
        rw_REG32 CORE_IRQ_CONTROL;              // [0x0c] VideoCore Interrupt Control
        rw_REG32 PMU_CONTROL_SET;               // [0x10] PMU Bit Set
        rw_REG32 PMU_CONTROL_CLR;               // [0x14] PMU Bit Clear
        ro_REG32 reserved1[3];
        rw_REG32 PERI_IRQ_ROUTE0;               // [0x24] Peripheral Interrupt Routing (Bank 0)
        ro_REG32 reserved2[2];
        rw_REG32 AXI_QUIET_TIME;                // [0x30] AXI Outstanding Transaction Time and IRQ Control
        rw_REG32 LOCAL_TIMER_CONTROL;           // [0x34] Local Timer Control
        rw_REG32 LOCAL_TIMER_IRQ;               // [0x38] Local Timer Reload and Interrupt
        ro_REG32 reserved3;
        rw_REG32 TIMER_CNTRL[4];                // [0x40] Timer Interrupt Control for ARM Core 0-3
        rw_REG32 MAILBOX_CNTRL[4];              // [0x50] Mailbox Interrupt Control for ARM Core 0-3
        rw_REG32 IRQ_SOURCE[4];                 // [0x60] IRQ Source flags for ARM Core 0-3
        rw_REG32 FIQ_SOURCE[4];                 // [0x70] FIQ Source flags for ARM Core 0-3
}irq_arm_local_regs_t;

typedef struct{
        /*
        IRQ/FIQ Enabled Interrupt Pending bits [31:0]
        IRQ/FIQ Enabled Interrupt pending bits [63:32]
        IRQ/FIQ Enabled Interrupt pending bits [79:64]
        */
        rw_REG32 PENDING[3];
        ro_REG32 reserved0;
        /*
        Write to Set IRQ/FIQ enable bits [31:0]
        Write to Set IRQ/FIQ enable bits [63:32]
        Write to Set IRQ/FIQ enable bits[79:64]
        */
        rw_REG32 SET_EN[3];
        ro_REG32 reserved1;
        /*
        Write to Clear IRQ/FIQ enable bits [31:0]
        Write to Clear IRQ/FIQ enable bits [63:32]
        Write to Clear IRQ/FIQ enable bits [79:64]
        */
        rw_REG32 CLR_EN[3];
        ro_REG32 reserved2;
}irq_config_regs_t;

typedef struct{
        ro_REG32 reserved[0x80];
        // ARM Core 0
        irq_config_regs_t core0_irq;
        /*
        Interrupt Line bits [31:0]
        Interrupt Line bits [63:32]
        Interrupt Line bits [79:64]
        */
        rw_REG32 STATUS[3];
        ro_REG32 reserved0;
        // ARM Core 1
        irq_config_regs_t core1_irq;
        ro_REG32 reserved1[4];
        // ARM Core 2
        irq_config_regs_t core2_irq;
        ro_REG32 reserved2[4];
        // ARM Core 3
        irq_config_regs_t core3_irq;
        ro_REG32 reserved3[4];
        // ARM Core 0
        irq_config_regs_t core0_fiq;
        ro_REG32 reserved4[4];
        // ARM Core 1
        irq_config_regs_t core1_fiq;
        ro_REG32 reserved5[4];
        // ARM Core 2
        irq_config_regs_t core2_fiq;
        ro_REG32 reserved6[4];
        // ARM Core 3
        irq_config_regs_t core3_fiq;
        rw_REG32 SWIRQ_SET;                     // [0x3f0] Write to Set Software Interrupt sources
        rw_REG32 SWIRQ_CLEAR;                   // [0x3f4] Write to Clear Software Interrupt sources
}irq_arm_c_regs_t;

#define IRQ_LOCAL_BASE          0xff800000     // in low peripheral mode
//#define IRQ_LOCAL_BASE          0x4c0000000     // in high peripheral mode, 35 bit full address map
#define IRQ_CORE_BASE           0x7e00b000

#define IRQ_L_REGS        ((volatile irq_arm_local_regs_t *)IRQ_LOCAL_BASE)
#define IRQ_C_REGS   ((volatile irq_arm_c_regs_t *)IRQ_CORE_BASE)

#define IRQ_CORE0_REGS ((volatile irq_config_regs_t *)IRQ_C_REGS->core0_irq)
#define IRQ_CORE1_REGS ((volatile irq_config_regs_t *)IRQ_C_REGS->core1_irq)
#define IRQ_CORE2_REGS ((volatile irq_config_regs_t *)IRQ_C_REGS->core2_irq)
#define IRQ_CORE3_REGS ((volatile irq_config_regs_t *)IRQ_C_REGS->core3_irq)

#define FIQ_CORE0_REGS ((volatile irq_config_regs_t *)IRQ_C_REGS->core0_fiq)
#define FIQ_CORE1_REGS ((volatile irq_config_regs_t *)IRQ_C_REGS->core1_fiq)
#define FIQ_CORE2_REGS ((volatile irq_config_regs_t *)IRQ_C_REGS->core2_fiq)
#define FIQ_CORE3_REGS ((volatile irq_config_regs_t *)IRQ_C_REGS->core3_fiq)

#endif // _ARMC_IRQ_H_
