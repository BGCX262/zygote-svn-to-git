;************************************
; 1st Stags BOOT LOADER             *
; writen by : vijaymohan avullapati *
;************************************

bits 16                         ;Intel processors starts in realmode
                                ;(which is 16 bit). "bits 16" directive
                                ;specifies the assembler to generate the code
                                ;for processor running in 16-bit mode.

org 0x7c00 			;Soon after the boot intel processors start 
				;executing the Instructions from memory 
				;location 0x7c00 
start: jmp loader

msgLoading  db 0x0D, 0x0A, "Loading Boot Image ", 0x0D, 0x0A, 0x00

Print:
			lodsb				; load next byte from string from SI to AL
			or	al, al			; Does AL=0?
			jz	PrintDone		; Yep, null terminator found-bail out
			mov	ah, 0eh			; Nope-Print the character
			int	10h
			jmp	Print			; Repeat until null terminator found
	PrintDone:
			ret				; we are done, so return

Reset:
        xor ax, ax                                  ; reset floppy disk function
        mov             dl, 0h                                   ; drive 0 is floppy drive
        int             0x13                                    ; call BIOS
        jc              Reset                                   ; If Carry Flag (CF) is set, there was an error. Try rese
        ret

Read:
        mov             ah, 0x02                                ; function 2
        mov             al, 2                                   ; read 1 sector
        mov             ch, 0                                   ; we are reading the second sector past us, so its still on track 1
        mov             cl, 2                                   ; sector to read (The second sector)
        mov             dh, 0                                   ; head number
        mov             dl, 0                                   ; drive number. Remember Drive 0 is floppy drive.
        int             0x13                                    ; call BIOS - Read the sector
        jc              Read
        ret

;-------------------------
;Boot Loader Entry Point -
;-------------------------
loader:
	;----------------------------------------------------
     	; code located at 7C00:0000, adjust segment registers
     	;----------------------------------------------------
	cli  			;disable interrupts
	mov ax, 0x00		;Make data segment & extended segments zero
	mov ds, ax		;as the addresses are based on 0x7c00:0000
	mov es, ax		;all the segments overlap with code segment.
	mov fs, ax
	mov gs, ax

	;----------------------------------------------------
     	; create stack
     	;----------------------------------------------------
     
        mov     ax, 0x0000				; set the stack
        mov     ss, ax
        mov     sp, 0xFFFF
        sti						; restore interrupts


	;----------------------------------------------------
	; Display loading message
	;----------------------------------------------------

	mov si, msgLoading		;move the start address of string to si.
	call Print     

	call Reset

	mov ax, 0x0050
	mov es, ax
	mov bx, 0x0000

	call Read

	jmp 0x0050:0x0000
times 510 - ($ - $$) db 0       ;Boot sector is of size 512 bytes. Fill the 
				;space after the last instruction i.e.,hlt 
				;until byte 510 with '0'.

dw 0xAA55			;Boot signature (If the 511 byte is 0xAA 
				;and the 512 byte is 0x55, INT 0x19 will 
				;load and execute the bootloader.).
