
#include "../arch/x86/cpu.h"
#include "../include/hw_init.h"
#include "../include/bootinfo.h"
#include "display.h"
#include "exception.h"


//! format of a memory region
struct memory_region {

	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	type;
	uint32_t	acpi_3_0;
};

//! different memory regions (in memory_region.type)
char* strMemoryTypes[] = {

	"Available",			//memory_region.type==0
	"Reserved",			//memory_region.type==1
	"ACPI Reclaim",			//memory_region.type==2
	"ACPI NVS Memory"		//memory_region.type==3
};

int _k_main(struct multiboot_info *bootinfo)
{
	int i;
	uint32_t memSize;
	struct memory_region*  region;
	uint32_t *p,*p2;

	/*Initialize hardware drivers*/
	hw_initialize();

	//! install our exception handlers
	setvect (0,  divide_by_zero_fault);
	setvect (1,  single_step_trap);
	setvect (2,  nmi_trap);
	setvect (3,  breakpoint_trap);
	setvect (4,  overflow_trap);
	setvect (5,  bounds_check_fault);
	setvect (6,  invalid_opcode_fault);
	setvect (7,  no_device_fault);
	setvect (8,  double_fault_abort);
	setvect (10, invalid_tss_fault);
	setvect (11, no_segment_fault);
	setvect (12, stack_fault);
	setvect (13, general_protection_fault);
	setvect (14, page_fault);
	setvect (16, fpu_fault);
	setvect (17, alignment_check_fault);
	setvect (18, machine_check_abort);
	setvect (19, simd_fpu_fault);

	ClrScr (0x13);
	GotoXY (0,0);
	SetColor (0x12);

	Puts ("\n		     zygote OS v0.01\n");
	SetColor (0x17);
/*	Puts ("Enabled A20!\n");
	Puts ("Initialized GDT and IDT!\n");
	Puts ("Installed PIC, PIT, and exception handlers!\n");
	Printf ("Cpu vender: %s \n", get_cpu_vender ());*/

	//! get memory size in KB
	memSize = 1024 + bootinfo->m_memoryLo + bootinfo->m_memoryHi*64;
	
	//! initialize the physical memory manager
	//! we place the memory bit map used by the PMM at the end of the kernel in memory
	pmmngr_init (memSize, 0x100000);

	Printf("pmm initialized with %i KB physical memory; memLo: %i memHi: %i\n",
	memSize,bootinfo->m_memoryLo,bootinfo->m_memoryHi);

	SetColor (0x19);
	Printf ("Physical Memory Map:\n");

	region = (struct memory_region*)0x1000;

	for (i=0; i<15; ++i) {

		//! sanity check; if type is > 4 mark it reserved
		if (region[i].type>4)
			region[i].type=1;

		//! if start address is 0, there is no more entries, break out
		if (i>0 && region[i].startLo==0)
			break;

		//! display entry
		Printf ("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %i (%s)\n", i, 
			region[i].startHi, region[i].startLo,
			region[i].sizeHi,region[i].sizeLo,
			region[i].type, strMemoryTypes[region[i].type-1]);

		//! if region is avilable memory, initialize the region for use
		if (region[i].type==1)
			pmmngr_init_region (region[i].startLo, region[i].sizeLo);
	}

	SetColor (0x17);

	Printf ("\npmm regions initialized: %i allocation blocks; used or reserved blocks: %i\nfree blocks: %i\n",
		pmmngr_get_block_count (),  pmmngr_get_use_block_count (), pmmngr_get_free_block_count () );

	//! allocating and deallocating memory examples...

	SetColor (0x12);

	p = (uint32_t*)pmmngr_alloc_block ();
	Printf ("\np allocated at 0x%x", p);

	p2 = (uint32_t*)pmmngr_alloc_blocks (2);
	Printf ("\nallocated 2 blocks for p2 at 0x%x", p2);

	pmmngr_free_block (p);
	p = (uint32_t*)pmmngr_alloc_block ();
	Printf ("\nUnallocated p to free block 1. p is reallocated to 0x%x", p);

	pmmngr_free_block (p);
	pmmngr_free_blocks (p2, 2);

	//To test divide by zero exception
	//i = 10/0;
	Puts ("\nHitting any key will fire the default handlers \n");
	for(;;) {
	}
	
};

