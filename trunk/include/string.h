#ifndef __STRING_H__
#define _STRING_H__
//****************************************************************************
//**
//**    [string.h]
//**    - Standard C String routines
//**
//****************************************************************************
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include "size_t.h"

//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

extern char *strcpy(char *s1, const char *s2);
extern size_t strlen ( const char* str );

extern void* memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short* memsetw(unsigned short *dest, unsigned short val, size_t count);

//****************************************************************************
//**
//**    END [string.h]
//**
//****************************************************************************

#endif
