#ifndef __IDT_H__
#define __IDT_H__

#include "stdint.h"


//! i86 defines 256 possible interrupt handlers (0-255)
#define I86_MAX_INTERRUPTS		256

//! must be in the format 0D110, where D is descriptor type
#define I86_IDT_DESC_BIT16		0x06	//00000110
#define I86_IDT_DESC_BIT32		0x0E	//00001110
#define I86_IDT_DESC_RING1		0x40	//01000000
#define I86_IDT_DESC_RING2		0x20	//00100000
#define I86_IDT_DESC_RING3		0x60	//01100000
#define I86_IDT_DESC_PRESENT		0x80	//10000000

//! interrupt handler w/o error code
//! Note: interrupt handlers are called by the processor. The stack setup may change
//! so we leave it up to the interrupts' implimentation to handle it and properly return
typedef void (*I86_IRQ_HANDLER)(void);

#define __packed

//! interrupt descriptor
struct idt_descriptor {

    //! bits 0-16 of interrupt routine (ir) address
    uint16_t		baseLo;

    //! code selector in gdt
    uint16_t		sel;
    
    //! reserved, shold be 0
    uint8_t		reserved;

    //! bit flags. Set with flags above
    uint8_t		flags;

    //! bits 16-32 of ir address
    uint16_t		baseHi;
}__attribute((packed));

//! describes the structure for the processors idtr register
struct idtr {

    //! size of the interrupt descriptor table (idt)
    uint16_t		limit;

    //! base address of idt
    uint32_t		base;
}__attribute((packed));

#undef __packed

//! returns interrupt descriptor
extern struct idt_descriptor* i86_get_ir (uint32_t i);


// initialize basic idt
extern int i86_idt_initialize (uint16_t codeSel);

#endif // __IDT_H__
