#!/usr/bin/awk -f

BEGIN{
    FS=",";
    structname=ARGV[2];
    if(structname == ""){
        structname="regs_t";
    }
    # dont process 2nd argument as a regular file.
    if(ARGC > 2){
        ARGC=2;
    }
    printf("typedef struct{\n");
    count=0;
}
{
    printf("%16s %-30s // [%s] %s\n","rw_REG32",$2";",$1,$3);
    name[count++]=$2;
}
END{
    printf("}%s;\n\n\n",structname);
    for(i=0;i<count;i++){
        printf("disp(%s);\n",name[i]);
    }
}
