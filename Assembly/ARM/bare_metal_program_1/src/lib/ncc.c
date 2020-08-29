int var1;


static int s_func2(void){
    return 0;
}


int func1(void){
    s_func2();
    return 0;
}
