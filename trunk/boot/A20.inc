
;********************************************
;	Enable A20 address line
;
;	OS Development Series
;********************************************

%ifndef __A20_H__
%define __A20_H__

bits	16

;-------------------------------------------------
; Enables a20 line through system control port A
;-------------------------------------------------

EnableA20_SysControlA:
	push	ax
	mov	al, 2
	out	0x92, al
	pop	ax
	ret

%endif ; __A20_H__

