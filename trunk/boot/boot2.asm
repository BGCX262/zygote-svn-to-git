;*********************************************
;	Stage2.asm
;		- Second Stage Bootloader
;
;
;*********************************************

org 0x0500					; offset to 0, we will set 
						; segments later

bits 16					; we are still in real mode

; we are loaded at linear address 0x0500

jmp smain				; jump to main

;*******************************************************
;       Preprocessor directives
;*******************************************************

%include "boot/stdio.inc"                       ; basic i/o routines
%include "boot/gdt.inc"                 ; Gdt routines
%include "boot/A20.inc"
%include "boot/memory.inc"


;*************************************************;
;       Data Section
;************************************************;

LoadingMsg      db      "Preparing to load operating system...",13,10,0

boot_info:
    istruc multiboot_info
	at multiboot_info.flags, 			dd 0 
	at multiboot_info.memoryLo,			dd 0
	at multiboot_info.memoryHi,			dd 0
	at multiboot_info.bootDevice,			dd 0
	at multiboot_info.cmdLine,			dd 0
	at multiboot_info.mods_count,			dd 0
	at multiboot_info.mods_addr,			dd 0
	at multiboot_info.syms0,			dd 0
	at multiboot_info.syms1,			dd 0
	at multiboot_info.syms2,			dd 0
	at multiboot_info.mmap_length,			dd 0
	at multiboot_info.mmap_addr,			dd 0
	at multiboot_info.drives_length,		dd 0
	at multiboot_info.drives_addr,			dd 0
	at multiboot_info.config_table,			dd 0
	at multiboot_info.bootloader_name,		dd 0
	at multiboot_info.apm_table,			dd 0
	at multiboot_info.vbe_control_info,		dd 0
	at multiboot_info.vbe_mode_info,		dw 0
	at multiboot_info.vbe_interface_seg,		dw 0
	at multiboot_info.vbe_interface_off,		dw 0
	at multiboot_info.vbe_interface_len,		dw 0

    iend

;*************************************************;
;	Second Stage Loader Entry Point
;************************************************;

Reset1:
        xor ax, ax                                  ; reset floppy disk function
        mov             dl, 0h                                   ; drive 0 is floppy drive
        int             0x13                                    ; call BIOS
        jc              Reset1                                   ; If Carry Flag (CF) is set, there was an error. Try rese
        ret

Read1:
        mov             ah, 0x02                                ; function 2
        mov             al, 40                                  ; read 40 sector
        mov             ch, 0                                   ; we are reading the second sector past us, so its still on track 1
        mov             cl, 4                                   ; sector to read (From fourth sector)
        mov             dh, 0                                   ; head number
        mov             dl, 0                                   ; drive number. Remember Drive 0 is floppy drive.
        int             0x13                                    ; call BIOS - Read the sector
        jc              Read1
        ret


smain:

	;-------------------------------;
	;   Setup segments and stack	;
	;-------------------------------;

	cli				; clear interrupts
	xor	ax, ax			; null segments
	mov	ds, ax
	mov	es, ax
	mov	ax, 0x9000		; stack begins at 0x9000-0xffff
	mov	ss, ax
	mov	sp, 0xFFFF
	sti				; enable interrupts

	mov     [boot_info+multiboot_info.bootDevice], dl
	;-------------------------------;
	;   Print loading message	;
	;-------------------------------;

	mov	si, LoadingMsg
	call	Puts16

	;-------------------------------;
	;   Install our GDT		;
	;-------------------------------;

	call	InstallGDT		; install our GDT

	;-------------------------------;
	;   Enable A20			;
	;-------------------------------;

	call	EnableA20_SysControlA

	xor		eax, eax
	xor		ebx, ebx
	call		BiosGetMemorySize64MB

	mov		word [boot_info+multiboot_info.memoryHi], bx
	mov		word [boot_info+multiboot_info.memoryLo], ax

	mov		eax, 0x0
	mov		ds, ax
	mov		di, 0x1000
	call		BiosGetMemoryMap

        call Reset1

        mov ax, 0x0800
        mov es, ax
        mov bx, 0x0000

        call Read1

	;-------------------------------;
	;   Go into pmode		;
	;-------------------------------;
	
	cli		; clear interrupts
	mov eax, cr0	; set bit 0 in cr0 -- enters pmode
	or eax, 1
	mov cr0, eax

	jmp CODE_DESC:Stage3	; far jump to fix CS

;******************************************************
;	ENTRY POINT FOR STAGE 3
;******************************************************

bits 32
Stage3:

	;-------------------------------;
	;   Set registers		;
	;-------------------------------;

	mov ax,  DATA_DESC	; set data segments to data selector (0x10)
	mov ds,  ax
	mov ss,  ax
	mov es,  ax
	mov esp, 90000h		; stack begins from 90000h

	;---------------------------------------;
	;   Clear screen and print success	;
	;---------------------------------------;

	;call		ClrScr32
	;mov		ebx, msg
	;call		Puts32

	push		dword boot_info

	jmp CODE_DESC:0x8000	;jump to the C main

msg db  0x0A, 0x0A, 0x0A, "               <[ Zygote OS 0.01 ]>"
    db  0x0A, 0x0A,             "           Basic 32 bit Operating System", 0


times 1024 - ($ - $$) db 0
