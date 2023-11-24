#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <wxlan/ioctls.h>

#define WXLAN_CON_FILE "/dev/wxcon"

// WXLAN_IOCTL_GET_BRIDGE_VLAN
// WXLAN_IOCTL_ADD_BRIDGE_PORT_TO_VLAN
// WXLAN_IOCTL_REM_BRIDGE_PORT_FROM_VLAN

int fd;

int getBridgeVlanChanged(uint16_t vlan){
	struct wxlan_bridge_vlan_changed_arg  a;
	int cmd, ret;

	memset(&a, 0x0, sizeof(struct wxlan_bridge_vlan_changed_arg));
	a.count = vlan;
	cmd = WXLAN_IOCTL_GET_BRIDGE_VLAN_CHANGED;

	printf("a=%p sizeof a=%d, %d, count=%d\n",&a, sizeof(a), sizeof(struct wxlan_bridge_vlan_changed_arg),a.count);
	ret = ioctl(fd, cmd, &a);

	if(ret == 0) {
		int i;
		printf("Count:%d\n",a.count);
		for(i=0; i<a.count; i++){
			if(i >=100){
				break;
			}
			printf("%d] %d\n",i,a.vlans[i]);
		}
	}else{
		printf("ioctl: %d returned\n", ret);
	}

	return ret;
}



int getBridgeVlan(uint16_t vlan){
	struct wxlan_bridge_vlan_arg a;
	int cmd, ret;

	memset(&a, 0x0, sizeof(struct wxlan_bridge_vlan_arg));
	cmd = WXLAN_IOCTL_GET_BRIDGE_VLAN;
	a.vlan = vlan;

	ret = ioctl(fd, cmd, &a);

	if(ret == 0) {
		printf("vlan %d portmask 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx\n", vlan, a.portmask[0], a.portmask[1], a.portmask[2], a.portmask[3], a.portmask[4]);
	}else{
		printf("ioctl: %d returned\n", ret);
	}

	return ret;
}

int addBridgeToVlan(uint16_t vlan, uint8_t port){
	struct wxlan_bridge_port_vlan_arg a;
	int cmd, ret;

	memset(&a, 0x0, sizeof(struct wxlan_bridge_port_vlan_arg));
	cmd = WXLAN_IOCTL_ADD_BRIDGE_PORT_TO_VLAN;
	a.vlan = vlan;
	a.port = port;

	ret = ioctl(fd, cmd, &a);

	if(ret == 0) {
		printf("addBridgeToVlan Success\n");
	}else{
		printf("ioctl: %d returned\n", ret);
	}

	return ret;

}

int remBridgeToVlan(uint16_t vlan, uint8_t port){
	struct wxlan_bridge_port_vlan_arg a;
	int cmd, ret;

	memset(&a, 0x0, sizeof(struct wxlan_bridge_port_vlan_arg));
	cmd = WXLAN_IOCTL_REM_BRIDGE_PORT_FROM_VLAN;
	a.vlan = vlan;
	a.port = port;

	ret = ioctl(fd, cmd, &a);

	if(ret == 0) {
		printf("remBridgeToVlan Success\n");
	}else{
		printf("ioctl: %d returned\n", ret);
	}

	return ret;
}

int main(){

//	uint16_t vlan;
  //      uint8_t port, op;

	int vlan, port, op;
	printf("NUM_BRIDGE_PORTS=%d %d\n",NUM_BRIDGE_PORTS,((NUM_BRIDGE_PORTS+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))));

	fd = open(WXLAN_CON_FILE, O_RDWR);
	if (fd < 0 ){
		printf("Unable to open %s\n", WXLAN_CON_FILE);
		return -1;
	}
	
	do{
                printf("\nop vlan Port:");
                scanf("%d",&op);
                scanf("%d",&vlan);
                scanf("%d",&port);
                //printf("op=%c vlan=%d, port=%d\n",op, vlan, port);
                if(op==1){
                        getBridgeVlan(vlan);
			printf("Adding port %d to vlan %d\n",port, vlan);
                        addBridgeToVlan(vlan, port);
                }else if(op==2){
                        getBridgeVlan(vlan);
			printf("Removing port %d from vlan %d\n",port, vlan);
                        remBridgeToVlan(vlan, port);
		}else if(op==3){
			getBridgeVlanChanged(vlan);
			break;
		}
                getBridgeVlan(vlan);
	}while(0);

	close(fd);
	return 0;
}
