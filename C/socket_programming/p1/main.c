#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include <signal.h>

int fd;
unsigned long pktcnt = 0;

int toHex(uint8_t c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else {
		return -1;
	}
}

int HexChar(uint8_t a, uint8_t b) {
	int ret = -1, msb, lsb;
	msb = toHex(a);
	lsb = toHex(b);
	if (msb < 0 || lsb < 0) {
		printf("Not Hex (0x%c%c)\n", a, b);
	} else {
		ret = (16 * msb);
		ret += lsb;
	}
	return ret;
}

int hexStr2Bytes(const uint8_t * ptr, uint8_t * buf) {
	int s_len = 0, i, j = 0, data;
	s_len = strlen(ptr);
	if (s_len % 2 != 0) {
		return -1;
	}
	for (i = 0; i < (s_len - 1); i += 2) {
		data = HexChar(ptr[i], ptr[i + 1]);
		if (data < 0) {
			j = -1;
			break;
		}
		buf[j++] = data;
//              printf("i=%d 0x%c%c => 0x%x  0x%x j=%d\n",i,ptr[i],ptr[i+1], data, buf[j-1], j-1);
	}
	return j;
}

void disp(uint8_t * p, int len, int isString) {
	int i;
	for (i = 0; i < len; i++) {
		if (isString) {
			printf("%c", p[i]);
		} else {
			printf("%02x", p[i]);
		}
	}
	printf("\n");
}

void sigHandler(int sig) {
	printf("Recieved signal %d, Closing fd %d\n", sig, fd);
	if (fd >= 0) {
		close(fd);
		fd = 0;
	}
	printf("%lu pkts sent\n", pktcnt);

	exit(0);
}

int main(int argc, char *argv[]) {
	int len, bytes_sent;
	int ttl = 255;
	long framesCnt = 0;
	long fps = 0;
	struct timespec ts;
	struct sockaddr_in dest_addr;
	uint8_t buf[200];
	uint8_t *dns_hexstr = "000084000000000400000000105f73706f746966792d636f6e6e656374045f746370056c6f63616c00000c00010000119400131030303161616531346135313630303030c00cc033001080010000119400160956455253494f4e3d310b43506174683d2f7a632f30c033002180010000007800130000000005780a363057616c6e75742d35c022c07a00018001000000780004c0a83c26";

	memset(&ts, 0x0, sizeof(ts));
	memset(buf, 0x0, 200 * (sizeof(uint8_t)));
	memset(&dest_addr, 0x0, sizeof(struct sockaddr_in));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(5353);
	dest_addr.sin_addr.s_addr = inet_addr("224.0.0.251");

	len = hexStr2Bytes(dns_hexstr, buf);
	if (len < 0) {
		printf("Unable to convert hex string to bytes\n");
		exit(1);
	}
	//printf("data_len=%d\n", len);

	if (argc >= 3) {
		framesCnt = strtol(argv[2], NULL, 10);
	}
	if (argc >= 4) {
		fps = strtol(argv[3], NULL, 10);
	}
	if (fps == 0) {
		fps = 3000;
	}
	pktcnt = 0;
	ts.tv_sec = 0;
	ts.tv_nsec = 1000000000 / (fps);
	printf("Frames#=%ld FPS=%ld  Interval=%ld ns\n", framesCnt, fps, ts.tv_nsec);

	signal(SIGHUP, sigHandler);
	signal(SIGABRT, sigHandler);
	signal(SIGKILL, sigHandler);
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);

	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd >= 0) {
		if (argc >= 2) {
			printf("iface=%s\n", argv[1]);
			if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, argv[1], strlen(argv[1])) < 0) {
				printf("Binding to interface %s failed\n", argv[1]);
			}
		}
		if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(&ttl)) < 0) {
			printf("Setting TTL to %d failed\n", ttl);
		}
		int iterCnt = framesCnt;
		while (1) {
			if (fd == 0) {
				break;
			}
			bytes_sent = sendto(fd, (void *)buf, len, 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
			if (bytes_sent == len) {
				++pktcnt;
			}
			nanosleep(&ts, NULL);
			if (framesCnt) {
				--iterCnt;
				if (iterCnt <= 0) {
					break;
				}
			}
		}
		//printf("Bytes_sent=%d\n", bytes_sent);

		close(fd);
		printf("%lu pkts sent\n", pktcnt);
	}
	return 0;
}
