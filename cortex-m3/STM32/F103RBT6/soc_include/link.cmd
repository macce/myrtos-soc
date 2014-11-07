#include "mem_map.h"

MEMORY
{
  vector_region (rx) : ORIGIN = VECTORS_START, LENGTH = VECTORS_SIZE
  start_stack_region (rw) : ORIGIN = START_STACK_START, LENGTH = START_STACK_SIZE
  kernel_data_region (rw) : ORIGIN = KERNEL_DATA_START, LENGTH = KERNEL_DATA_SIZE
  memory_region (rx) : ORIGIN = MEMORY_START, LENGTH = MEMORY_SIZE
}

SECTIONS
{
  .vectors :
  {
    *(.vectors);     /* Vector table */
  } > vector_region

  .start_stack :
  {
    . += START_STACK_SIZE; 
    _start_stack_end = .;
  } > start_stack_region

  .kernel_data :
  { 
    _kernel_pool_start = .;
    . += KERNEL_DATA_SIZE;
    _kernel_pool_end = .;
  } > kernel_data_region
  
  .text :
  {
    *(.text)         /* Program code */
    *(.rodata)       /* Read only data */
  } > memory_region

  .data :
  {
    *(.data)         /* Data memory */
  } > memory_region
  
  .bss :
  {
    _bss_start = .;
    *(.bss)
    *(COMMON)
    _bss_end = .;
  } > memory_region
}  
