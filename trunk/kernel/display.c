//****************************************************************************
//**
//**    display.c
//**    - Provides display capabilities for debugging. Because it is
//**	  specifically for debugging and not final release, we don't
//** 	  care for portability here
//**
//****************************************************************************
//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "../include/hw_init.h"

//! video memory
uint16_t *video_memory = (uint16_t *)0xB8000;

//! current position
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

//! current color
uint8_t	_color=0;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

#ifdef _MSC_VER
#pragma warning (disable:4244)
#endif

//! Displays a character
void Putc (unsigned char c) {

    uint16_t attribute = _color << 8;

    //! backspace character
    if (c == 0x08 && cursor_x)
        cursor_x--;

    //! tab character
    else if (c == 0x09)
        cursor_x = (cursor_x+8) & ~(8-1);

    //! carriage return
    else if (c == '\r')
        cursor_x = 0;

    //! new line
	else if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
	}

    //! printable characters
    else if(c >= ' ') {

		//! display character on screen
        uint16_t* location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    //! if we are at edge of row, go to new line
    if (cursor_x >= 80) {

        cursor_x = 0;
        cursor_y++;
    }
}


char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void itoa(unsigned i,unsigned base,char* buf) {
   int pos = 0;
   int opos = 0;
   int top = 0;

   if (i == 0 || base > 16) {
      buf[0] = '0';
      buf[1] = '\0';
      return;
   }

   while (i != 0) {
      tbuf[pos] = bchars[i % base];
      pos++;
      i /= base;
   }
   top=pos--;
   for (opos=0; opos<top; pos--,opos++) {
      buf[opos] = tbuf[pos];
   }
   buf[opos] = 0;
}

void itoa_s(int i,unsigned base,char* buf) {
   if (base > 16) return;
   if (i < 0) {
      *buf++ = '-';
      i *= -1;
   }
   itoa(i,base,buf);
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//! Sets new font color
unsigned SetColor (const unsigned c) {

	unsigned t=_color;
	_color=c;
	return t;
}

//! Sets new position
void GotoXY (unsigned x, unsigned y) {

	if (cursor_x <= 80)
	    cursor_x = x;

	if (cursor_y <= 25)
	    cursor_y = y;
}

//! Clear screen
void ClrScr (const uint8_t c) {

	int i;
	//! clear video memory by writing space characters to it
	for (i = 0; i < 80*25; i++)
        video_memory[i] = ' ' | (c << 8);

    //! move position back to start
    GotoXY (0,0);
}

//! Displays a string
void Puts (char* str) {

	unsigned int i;
	if (!str)
		return;

	//! err... displays a string
    for (i=0; i<strlen(str); i++)
        Putc (str[i]);
}

//! Displays a formatted string
int Printf (const char* str, ...) {

	char ch;
	int c;
	if(!str)
		return 0;

	va_list		args;
	va_start (args, str);
	size_t i;
	for (i=0; i<strlen(str);i++) {

		switch (str[i]) {

			case '%':

				switch (str[i+1]) {

					/*** characters ***/
					case 'c': {
						ch = (char)va_arg (args, int);
						Putc (ch);
						i++;		// go to next character
						break;
					}

					/*** address of ***/
					case 's': {
						c = (int)va_arg (args, int);
						char str[64];
						strcpy (str,(const char*)c);
						Puts (str);
						i++;		// go to next character
						break;
					}

					/*** integers ***/
					case 'd':
					case 'i': {
						c = va_arg (args, int);
						char str[32]={0};
						itoa_s (c, 10, str);
						Puts (str);
						i++;		// go to next character
						break;
					}

					/*** display in hex ***/
					case 'X':
					case 'x': {
						c = va_arg (args, int);
						char str[32]={0};
						itoa_s (c,16,str);
						Puts (str);
						i++;		// go to next character
						break;
					}

					default:
						va_end (args);
						return 1;
				}

				break;

			default:
				Putc (str[i]);
				break;
		}

	}

	va_end (args);
	return i;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[display.c]
//**
//****************************************************************************
