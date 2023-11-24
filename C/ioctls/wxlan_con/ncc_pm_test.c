#include<stdio.h>
#include <string.h>
#include<stdint.h>



/**
 * test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
#define BITS_PER_LONG 64
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
static inline int test_bit(int nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}

/**
 * __set_bit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static inline void __set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p  |= mask;
}

static inline void __clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p &= ~mask;
}






#define NUM_BRIDGE_PORTS 134

typedef unsigned long bridge_portmask_t[(NUM_BRIDGE_PORTS + 8 * sizeof(unsigned long) - 1) / (8 * sizeof(unsigned long))];

/**
 * per-vlan information
 */
struct bridge_vlan {
	bridge_portmask_t portmask;	///< bitmap of ports belonging to this vlan 
	bridge_portmask_t tagmask;	///< bitmap of ports which desired tagged packets from this vlan (bits not set in portmask don't make logical sense, and you probably shouldn't set them)
};

static struct bridge_vlan bridge_vlans[4096];

/**
 * get the portmask of a vlan
 */
struct wxlan_bridge_vlan_arg {
	uint16_t vlan;		///< the Port vlan-ID (host-endian)
	uint32_t portmask[(NUM_BRIDGE_PORTS + 31) / 32];	///< Port Mask
};

/**
 * add/remove one port from a vlan
 */
struct wxlan_bridge_port_vlan_arg {
	uint16_t vlan;		///< The Port Vlan-ID (host-endian)
	uint8_t port;		///< The port ID.
};

void add_port(uint16_t vlan, int port) {
                                struct wxlan_bridge_port_vlan_arg a;
				a.port = port;
				a.vlan = vlan;
                                if (a.port >= NUM_BRIDGE_PORTS)
                                        return ;
                                if (a.vlan == 0 || a.vlan >= 4095) // note: 0 and 4095 are not allowed to be set (you can query them all you want)
                                        return ;

                                if (!test_bit(a.port, bridge_vlans[a.vlan].portmask)) {
                                        __set_bit(a.port, bridge_vlans[a.vlan].portmask);
                                } // else the port is already in the vlan

                                return ;



}
void rem_port(uint16_t vlan, int port) {
                                struct wxlan_bridge_port_vlan_arg a;
				a.port = port;
				a.vlan = vlan;
                                if (a.port >= NUM_BRIDGE_PORTS)
                                        return ;
                                if (a.vlan == 0 || a.vlan >= 4095) // note: 0 and 4095 are not allowed to be set (you can query them all you want)
                                        return ;
				
                                if (test_bit(a.port, bridge_vlans[a.vlan].portmask)) {
                                        __clear_bit(a.port, bridge_vlans[a.vlan].portmask);
                                } // else the port is already absent from the vlan

}

void get_pm(uint16_t vlan) {
	int i;
	struct wxlan_bridge_vlan_arg a;
	memset(&a, 0, sizeof(a));
	a.vlan = vlan;
	if (a.vlan >= 4096)
		return;
	memset(a.portmask, 0, sizeof(a.portmask));
	struct bridge_vlan *const v = bridge_vlans + a.vlan;

	for (i = 0; i < NUM_BRIDGE_PORTS; i++)
		if (test_bit(i, v->portmask))
			a.portmask[i >> 5] |= (uint32_t) 1 << (i & 31);

	printf("vlan=%d portmask 0x%llx 0x%llx 0x%llx  b 0x%lx 0x%lx 0x%lx\n", a.vlan, (unsigned long long)a.portmask[0], (unsigned long long)a.portmask[1], (unsigned long long)a.portmask[2],v->portmask[0],v->portmask[1],v->portmask[2]);
}

int main() {

	uint16_t vlan;
	int port, op;
	memset(&bridge_vlans, 0x0, sizeof(struct bridge_vlan));
	//printf("pm=%lu sizeof=%lu, %lu\n",*pm,sizeof(pm),sizeof(bridge_portmask_t));  

	do{
		printf("op vlan Port:");
		scanf("%c %hu %d",&op, &vlan, &port);
		printf("vlan=%d, port=%d\n",vlan, port);
		if(op=='a'){
			get_pm(vlan);
			add_port(vlan, port);
		}else if(op=='r'){
			get_pm(vlan);
			rem_port(vlan, port);
		}
		get_pm(vlan);
	}while(op != '0');
	return 0;
}
