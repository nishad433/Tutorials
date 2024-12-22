#if 0
void memclr(void *s, long unsigned int n){
	memset(s, 0x0, n);
}

void memset(void *s, int c, long unsigned int n){
	while(n--){
		*((char *)(s+n)) = (char)c;
	}
}

void memcpy(void *dest, const void *src, long unsigned int n){

}
#endif
