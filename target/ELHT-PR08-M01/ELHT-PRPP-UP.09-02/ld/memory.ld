/*
Linker-скрипт для CH32V307VC от WCH из примеров.
*/
ENTRY( _start )

__stack_size = 2048;
_estack = ORIGIN(RAM) + LENGTH(RAM);
__freertos_irq_stack_top = _estack;

PROVIDE( _stack_size = __stack_size );

__retain_size = 4096;

MEMORY
{
  BOOT_DATA (rx) :
    ORIGIN = 0x00000000+16384-256,
    LENGTH = 256
  FLASH (rx) :
    ORIGIN = 0x00000000+16384,
    LENGTH = 208K
  RAM (xrw) :
    ORIGIN = 0x20000000,
    LENGTH = 96K - __retain_size
  RETAIN (xrw) :
    ORIGIN = 0x20000000 + LENGTH(RAM),
    LENGTH = __retain_size
}


SECTIONS
{
  .init :
  {
    _sinit = .;
    . = ALIGN(4);
    KEEP(*(SORT_NONE(.init)))
    . = ALIGN(4);
    _einit = .;
  } >FLASH AT>FLASH

  .vector :
  {
    *(.vector);
    . = ALIGN(64);
  } >FLASH AT>FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text.*)
    *(.rodata)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    *(.gnu.linkonce.t.*)
    . = ALIGN(4);
  } >FLASH AT>FLASH 

  .fini :
  {
    KEEP(*(SORT_NONE(.fini)))
    . = ALIGN(4);
  } >FLASH AT>FLASH

  PROVIDE( _etext = . );
  PROVIDE( _eitcm = . );	

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH AT>FLASH 

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)))
    KEEP (*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH AT>FLASH 
  
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
    KEEP (*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH AT>FLASH 

  .shared :
  {
    . = ALIGN(4);
    PROVIDE(__sshared_start = .);
    KEEP(*(.shared.bootloader))
    . = ALIGN(4);
    *.o (shared)
    KEEP(*(.shared))
    . = 1024;
    PROVIDE(__sshared_end = .);
  } >RAM AT>FLASH

  .dalign :
  {
    . = ALIGN(4);
    PROVIDE(_data_vma = .);
  } >RAM AT>FLASH	

  .dlalign :
  {
    . = ALIGN(4); 
    PROVIDE(_data_lma = .);
  } >FLASH AT>FLASH

  .data :
  {
    *(.gnu.linkonce.r.*)
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.*)
    *(.sdata2.*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
    . = ALIGN(4);
    PROVIDE( _edata = .);
  } >RAM AT>FLASH

  .bss :
  {
    . = ALIGN(4);
    *(.noinit*)
    . = ALIGN(4);
    PROVIDE( _sbss = .);
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss*)
    *(.gnu.linkonce.b.*)		
    *(COMMON*)
    . = ALIGN(4);
    PROVIDE( _ebss = .);
  } >RAM AT>FLASH

  PROVIDE( _end = _ebss);
  PROVIDE( end = . );

  .stack :
  {
    . = ALIGN(4);
    . = . + __stack_size;
  } >RAM

  /* BOOTLOADER */

  .boot_data :
  {
    . = ALIGN(4);
    PROVIDE(_sbootdata = .);
    . += 128;
    *.o (boot_data)
    KEEP(*(.boot_data))
    . = ALIGN(256);
    PROVIDE(_ebootdata = .);
  } >BOOT_DATA

  /* RETAIN */

  .eeprom_align :
  {
    PROVIDE(_sretain_lma = .);
  } >FLASH AT>FLASH

  .eeprom :
  {
    PROVIDE( _sretain_vma = . );
    *.o (eeprom)
    KEEP(*(.eeprom))
    . = ALIGN(4);
    PROVIDE( _eretain_vma = . );
  } >RETAIN AT>FLASH
}
