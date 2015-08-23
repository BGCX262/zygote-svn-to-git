#ifndef __DISPLAY_H__
#define __DISPLAY_H__
//****************************************************************************
//**
//**    display.h
//**    - Provides display capabilities for debugging. Because it is
//**	  specifically for debugging and not final release, we don't
//** 	  care for portability here
//**
//****************************************************************************

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>

extern void Putc (unsigned char c);
extern void ClrScr (const uint8_t c);
extern void Puts (char* str);
extern int Printf (const char* str, ...);
extern unsigned SetColor (const unsigned c);
extern void GotoXY (unsigned x, unsigned y);

//****************************************************************************
//**
//**    END [display.h]
//**
//****************************************************************************
#endif
