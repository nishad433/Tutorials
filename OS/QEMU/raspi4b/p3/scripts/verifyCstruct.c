#include <stdint.h>
#include <stdio.h>

typedef uint32_t rw_REG32;
typedef const uint32_t ro_REG32;


typedef struct{
    rw_REG32 GICD_CTLR;                         // 0x000 Distributor Control Register
    ro_REG32 GICD_TYPER;                        // 0x004 Interrupt Controller Type Register
    ro_REG32 GICD_IIDR;                         // 0x008 Distributor Implementer Identification Register
    uint32_t reserved0[0x1D];
    rw_REG32 GICD_IGROUPRn[15];                 // 0x080-0x0BC Interrupt Group Registers
    uint32_t reserved1[0x11];
    rw_REG32 GICD_ISENABLERn[15];               // 0x100-0x13C Interrupt Set-Enable Registers
    uint32_t reserved2[0x11];
    rw_REG32 GICD_ICENABLERn[15];               // 0x180-0x1BC Interrupt Clear-Enable Registers
    uint32_t reserved3[0x11];
    rw_REG32 GICD_ISPENDRn[15];                 // 0x200-0x23C Interrupt Set-Pending Registers
    uint32_t reserved4[0x11];
    rw_REG32 GICD_ICPENDRn[15];                 // 0x280-0x2BC Interrupt Clear-Pending Registers
    uint32_t reserved5[0x11];
    rw_REG32 GICD_ISACTIVERn[15];               // 0x300-0x33C Interrupt Set-Active Registers
    uint32_t reserved6[0x11];
    rw_REG32 GICD_ICACTIVERn[15];               // 0x380-0x3BC Interrupt Clear-Active Registers
    uint32_t reserved7[0x11];
    rw_REG32 GICD_IPRIORITYRn[128];             // 0x400-0x5FC Interrupt Priority Registers
    uint32_t reserved8[0x80];
    ro_REG32 GICD_ITARGETSRn[128];              // 0x800-0x81C Interrupt Processor Targets Registers
    uint32_t reserved9[0x80];
    rw_REG32 GICD_ICFGRn[32];                   // 0xC00-0xC7C Interrupt Configuration Registers
    uint32_t reserved10[0x20];
    ro_REG32 GICD_PPISR;                        // 0xD00 Private Peripheral Interrupt Status Register,
    ro_REG32 GICD_SPISRn[15];                   // 0xD04-0xD3C Shared Peripheral Interrupt Status Registers,
    uint32_t reserved11[0x70];
    rw_REG32 GICD_SGIR;                         // 0xF00 Software Generated Interrupt Register
    uint32_t reserved12[0x3];
    rw_REG32 GICD_CPENDSGIRn[4];                // 0xF10-0xF1C SGI Clear-Pending Registers
    rw_REG32 GICD_SPENDSGIRn[4];                // 0xF20-0xF2C SGI Set-Pending Registers
    uint32_t reserved13[0x28];
    rw_REG32 GICD_PIDR4;                        // 0xFD0 Peripheral ID 4 Register
    rw_REG32 GICD_PIDR5;                        // 0xFD4 Peripheral ID 5 Register
    rw_REG32 GICD_PIDR6;                        // 0xFD8 Peripheral ID 6 Register
    rw_REG32 GICD_PIDR7;                        // 0xFDC Peripheral ID 7 Register
    rw_REG32 GICD_PIDR0;                        // 0xFE0 Peripheral ID 0 Register
    rw_REG32 GICD_PIDR1;                        // 0xFE4 Peripheral ID 1 Register
    rw_REG32 GICD_PIDR2;                        // 0xFE8 Peripheral ID 2 Register
    rw_REG32 GICD_PIDR3;                        // 0xFEC Peripheral ID 3 Register
    rw_REG32 GICD_CIDR0;                        // 0xFF0 Component ID 0 Register
    rw_REG32 GICD_CIDR1;                        // 0xFF4 Component ID 1 Register
    rw_REG32 GICD_CIDR2;                        // 0xFF8 Component ID 2 Register
    rw_REG32 GICD_CIDR3;                        // 0xFFC Component ID 3 Register
}gic_dist_regs_t;



typedef struct{
    rw_REG32 GICC_CTLR;         // 0x0000 CPU Interface Control Register
    rw_REG32 GICC_PMR;          // 0x0004 Interrupt Priority Mask Register
    rw_REG32 GICC_BPR;          // 0x0008 Binary Point Register
    rw_REG32 GICC_IAR;          // 0x000C Interrupt Acknowledge Register
    rw_REG32 GICC_EOIR;         // 0x0010 End of Interrupt Register
    rw_REG32 GICC_RPR;          // 0x0014 Running Priority Register
    rw_REG32 GICC_HPPIR;        // 0x0018 Highest Priority Pending Interrupt Register
    rw_REG32 GICC_ABPR;         // 0x001C Aliased Binary Point Register
    rw_REG32 GICC_AIAR;         // 0x0020 Aliased Interrupt Acknowledge Register
    rw_REG32 GICC_AEOIR;        // 0x0024 Aliased End of Interrupt Register
    rw_REG32 GICC_AHPPIR;       // 0x0028 Aliased Highest Priority Pending Interrupt Registerc
    uint32_t reserved0[0x29];
    rw_REG32 GICC_APR0;         // 0x00D0 Active Priority Register
    uint32_t reserved1[0x3];
    rw_REG32 GICC_NSAPR0;       // 0x00E0 Non-Secure Active Priority Register
    uint32_t reserved2[0x6];
    rw_REG32 GICC_IIDR;         // 0x00FC CPU Interface Identification Register
    uint32_t reserved3[0x3C0];
    rw_REG32 GICC_DIR;          // 0x1000 Deactivate Interrupt Register
}gic_cpu_intf_regs_t;



typedef struct{
    uint32_t reserved[0x400];
    gic_dist_regs_t distributor;
    gic_cpu_intf_regs_t cpu_intf[4];
    uint32_t test;
}gic400_regs_t;


//#define disp(x)  printf("%-30s 0x%02lx\n",#x,(uint64_t)(&((regs_t *)0)->x));
#define disp(x) printf("0x%02lx\t%s\n", (uint64_t)(&((gic400_regs_t *)0)->x), #x);
#define disp1(x) printf("0x%03lx\t%s\n", (uint64_t)(&((gic_dist_regs_t *)0)->x), #x);
#define disp2(x) printf("0x%03lx\t%s\n", (uint64_t)(&((gic_cpu_intf_regs_t *)0)->x), #x);


int main() {

    int index=0;
  //  disp(distributor);

	disp1(GICD_CTLR);
	disp1(GICD_TYPER);
	disp1(GICD_IIDR);
	disp1(GICD_IGROUPRn[0]);
	disp1(GICD_IGROUPRn[14]);
	disp1(GICD_ISENABLERn[0]);
	disp1(GICD_ISENABLERn[1]);
	disp1(GICD_ISENABLERn[14]);
	disp1(GICD_ICENABLERn[0]);
	disp1(GICD_ICENABLERn[1]);
	disp1(GICD_ICENABLERn[14]);
	disp1(GICD_ISPENDRn[0]);
	disp1(GICD_ISPENDRn[14]);
	disp1(GICD_ICPENDRn[0]);
	disp1(GICD_ICPENDRn[14]);
	disp1(GICD_ISACTIVERn[0]);
	disp1(GICD_ISACTIVERn[14]);
	disp1(GICD_ICACTIVERn[0]);
	disp1(GICD_ICACTIVERn[14]);
	disp1(GICD_IPRIORITYRn[0]);
	disp1(GICD_IPRIORITYRn[127]);
	disp1(GICD_ITARGETSRn[0]);
	disp1(GICD_ITARGETSRn[7]);
	disp1(GICD_ITARGETSRn[8]);
	disp1(GICD_ITARGETSRn[127]);
	disp1(GICD_ICFGRn[0]);
	disp1(GICD_ICFGRn[1]);
	disp1(GICD_ICFGRn[31]);
	disp1(GICD_PPISR);
	disp1(GICD_SPISRn[0]);
	disp1(GICD_SPISRn[14]);
	disp1(GICD_SGIR);
	disp1(GICD_CPENDSGIRn[0]);
	disp1(GICD_CPENDSGIRn[2]);
	disp1(GICD_SPENDSGIRn[0]);
	disp1(GICD_SPENDSGIRn[2]);
	disp1(GICD_PIDR4);
	disp1(GICD_PIDR5);
	disp1(GICD_PIDR6);
	disp1(GICD_PIDR7);
	disp1(GICD_PIDR0);
	disp1(GICD_PIDR1);
	disp1(GICD_PIDR2);
	disp1(GICD_PIDR3);
	disp1(GICD_CIDR0);
	disp1(GICD_CIDR1);
	disp1(GICD_CIDR2);
	disp1(GICD_CIDR3);

    disp(cpu_intf);
    disp2(GICC_CTLR);
    disp2(GICC_PMR);
    disp2(GICC_BPR);
    disp2(GICC_IAR);
    disp2(GICC_EOIR);
    disp2(GICC_RPR);
    disp2(GICC_HPPIR);
    disp2(GICC_ABPR);
    disp2(GICC_AIAR);
    disp2(GICC_AEOIR);
    disp2(GICC_AHPPIR);
    disp2(GICC_APR0);
    disp2(GICC_NSAPR0);
    disp2(GICC_IIDR);
    disp2(GICC_DIR);
    disp(test);


  return 0;
}
