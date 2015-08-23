//****************************************************************************
////**
////**    cpu.c
////**
////**	This is the processor interface. Everything outside of this module
////**	must use this interface when working on processor data.
////**
////**	A processor is a module that manages the very basic data structures
////**	and data within the system. The processor interface provides the interface
////**	for managing processors, processor cores, accessing processor structures,
////**	and more
////**
////****************************************************************************

#include "idt.h"

//! Initializes cpu resources
int i86_cpu_initialize () {

    //! initialize processor tables
    i86_idt_initialize (0x08);

    return 0;
}

//! shuts down cpu resources...Nothing to do yet
void i86_cpu_shutdown () {


}
char* i86_cpu_get_vender () {

	static char	str[32] = {0};
	char str1[4] = {0};
	char str2[4] = {0};
	asm("mov $0x0, %eax");
	asm("cpuid");
	asm("mov %%ebx, %0\n"
	    "mov %%edx, %1\n"
	    "mov %%ecx, %2\n"
	    :"=m"(str),"=m"(str1),"=m"(str2));
	strcpy(str+4,str1);
	strcpy(str+8,str2);	
	str[12] = '\0';

	return str;
}

