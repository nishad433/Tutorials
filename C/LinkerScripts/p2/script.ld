ENTRY(main)
OUTPUT_FORMAT(elf64-x86-64)






SECTIONS
{

    .text : 
    {
	_stext = . ;
	*(.text)
	_etext = . ;
    }

    .data :
    {
	_sdata = . ;
	*(.data)
	_edata = . ;
    } 

}
