#include <linux/if_vlan.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>


int nccdebug_level = 0; 
int dumpSKB = 0; 
module_param(nccdebug_level, int, 0644); 
module_param(dumpSKB, int, 0644); 
MODULE_PARM_DESC(nccdebug_level,"ncc debug level"); 
MODULE_PARM_DESC(dumpSKB, "dump SKB data"); 


static void dump_skb(const struct sk_buff *const skb, int size, int hdr_decode, const char *func, int line) {
        const char *const d = (const char *)skb->data;
        const struct vlan_ethhdr *const e = (struct vlan_ethhdr *)d;
        pr_info("%s:%d skb(%p) dev %s skb->data(%p)", func, line, skb, skb->dev->name, skb->data);

        // decode skb->data only if specified else just dump raw contents.
        if (likely(hdr_decode)) {
                // dump some relavant information from skb
                if (unlikely(e->h_vlan_proto == htons(ETH_P_8021Q))) {
                        pr_info("proto(0x%04x) vlan %d\n", htons(e->h_vlan_proto), htons(e->h_vlan_TCI));
                }
                pr_info("smac(%pM) dmac(%pM) \n", e->h_source, e->h_dest);
        }
        print_hex_dump(KERN_INFO, "data: ", DUMP_PREFIX_ADDRESS, 16, 1, d, size, true);
        printk("\n");
}

 
// debug interface by default compiled out. 
#define nccdprintf(x,...) do { if (unlikely(nccdebug_level)) pr_info(x, ##__VA_ARGS__); } while (0) 
#define nccddprintf(x,...) do { if (unlikely(nccdebug_level > 1)) pr_info(x, ##__VA_ARGS__); } while (0) 
#define nccdddprintf(x,...) do { if (unlikely(nccdebug_level >2)) pr_info(x, ##__VA_ARGS__); } while (0) 
#define nccprint_hex_dump_bytes(prefix, flags, ptr, len) do { if (unlikely(dumpSKB)) print_hex_dump_bytes((prefix), (flags), (ptr), (len)); } while(0) 
#define nccdump_skb(skb, size, hdr_decode, func, line) do { if (unlikely(dumpSKB)) dump_skb(skb, size, hdr_decode, func, line); } while (0) 
#define nccddump_skb(skb, size, hdr_decode, func, line) do { if (unlikely(dumpSKB>1)) dump_skb(skb, size, hdr_decode, func, line); } while (0) 


#define NONE_SMAC 0x0
#define ROUTER_SMAC 0x1
#define	SWITCH_SMAC 0x2
#define WC_SMAC 0x3
#define WL_SMAC 0x4

int isDHCPPkt(const struct sk_buff *skb){
        const struct vlan_ethhdr *evh = skb->data;
        uint16_t proto;
        uint16_t ethhdr_size = 14;
        int ret = 0;

        proto = ntohs(evh->h_vlan_proto);
        if(proto == ETH_P_8021Q){
                proto = ntohs(evh->h_vlan_encapsulated_proto);
                ethhdr_size += 4;
        }

        if(proto == ETH_P_IP){
                // IP Packet
                struct iphdr *iph = (struct iphdr *)((char*)skb->data + ethhdr_size);
                if(iph->protocol == IPPROTO_UDP){
                        // UDP Packet
                        struct udphdr* udph = (struct udphdr*)((char*)iph + 4*iph->ihl);
                        if(udph->source == htons(67) || udph->source == htons(68)){
                                // DHCP Packet
                                ret = 1;
                        }
                }
        }
        return ret;
}
EXPORT_SYMBOL(isDHCPPkt);


int ncc_debug(const struct sk_buff *skb, int tx, const char *func, int line){
        const struct vlan_ethhdr *evh = skb->data;
        uint16_t vlan_id = 0;
        int status=0, i;
        uint8_t *mac = NULL;
        uint8_t router_mac[]={0xb8,0x27,0xeb,0x46};
        uint8_t ng_mac[]={0xb0,0x39,0x56,0x7c,0x5d,0x1e};
        uint8_t wc_mac[2][ETH_ALEN]={
                {0x00,0x13,0x3b,0x00,0x06,0xa2},
                {0x00,0xe0,0x4c,0x66,0x11,0x66}
        };

        if(tx){
                mac = evh->h_source;
        }else{
                mac = evh->h_source;
        }

        if(!memcmp(mac, router_mac, 0x4)){
                status = ROUTER_SMAC;
        }else if(!memcmp(mac, ng_mac, ETH_ALEN)){
                status = SWITCH_SMAC;
        }else{
                for(i=0; i<2; i++){
                        if(!memcmp(mac, wc_mac[i], ETH_ALEN)){
                                status = WC_SMAC;
                                break;
                        }
                }
        }

	if(status){
			uint16_t vlan_id = 0;
			int isdhcpPkt;
			if (evh->h_vlan_proto == htons(ETH_P_8021Q)){
				 vlan_id = ntohs(evh->h_vlan_TCI & htons(VLAN_VID_MASK));
			}

			isdhcpPkt = isDHCPPkt(skb);
			if(isdhcpPkt){
				nccdprintf("NCC[%s:%d] skb=%px SMAC=%pM DMAC=%pM vlan_id=%d, ext_port=%d isDHCP=%d\n",func, line, skb, evh->h_source,evh->h_dest,vlan_id,skb->mist_ext_port,isdhcpPkt);
				nccdump_skb(skb, 64, false, func, line);
			}else{
				if(tx != 2){
					nccddprintf("NCC[%s:%d] skb=%px SMAC=%pM DMAC=%pM vlan_id=%d, ext_port=%d isDHCP=%d\n",func, line, skb, evh->h_source,evh->h_dest,vlan_id,skb->mist_ext_port,isdhcpPkt);
					nccddump_skb(skb, 64, false, func, line);
				}
			}

	}



        return status;
}

EXPORT_SYMBOL(ncc_debug);
