;kernel.asm
;Michael Black, 2007

;kernel.asm contains assembly functions that you can use in your kernel

	.global _launchProgram
	.global _moveCursor
	.global _clearScreen
	.global _readSector
	.global _readChar
	.global _printChar
	.global _interrupt
	.global _putInMemory
	.global _makeInterrupt21
	.global _interrupt21ServiceRoutine
	.global _end
	.global _callPrintString
	.global _callReadString
	.global _callReadSector
	.global _loadProgram
	.extern _printString
	.extern _readString
	.extern _readFile

;this is called to start a program that is loaded into memory
;void launchProgram(int segment)
_launchProgram:
	mov bp,sp
	mov bx,[bp+2] ;get the segment into bx
	mov ax,cs	;modify the jmp below to jump to our segment
	mov ds,ax	;this is self-modifying code
	mov si,#jump
	mov [si+3],bx	;change the first 0000 to the segment
	mov ds,bx	;set up the segment registers
	mov ss,bx
	mov es,bx
	mov sp,#0xfff0 ;set up the stack pointer
	mov bp,#0xfff0

jump: jmp #0x0000:0x0000 ;and start running (the first 0000 is changed above)


_moveCursor:
	mov bh, #0
	mov ah, #2
	mov dh, #0
	mov dl, #0
	int #0x10
	ret

_clearScreen:
	mov dx, #0	; Position cursor at top-left
	mov ah, #6	; Scroll full-screen
	mov al, #0	; Normal white on black
	mov bh, #7	;
	mov cx, #0	; Top-left
	mov dh, #24	; Bottom-right
	mov dl, #79
	int #0x10
	call _moveCursor
	ret

_readSector:
	push bp
	mov bp,sp
	sub sp,#6
	mov bx,[bp+4]   
	mov ax,[bp+6]	
	mov cl,#36
	div cl
	xor ah,ah
	mov [bp-2],ax  	
	mov ax,[bp+6]
	mov cl,#18
	div cl
	and al,#0x1
	xor dx,dx
	mov dl,al
	mov [bp-4],dx  
	add ah,#1
	xor dx,dx
	mov dl,ah
	mov [bp-6], dx  
	mov al,#0x1
	mov dl, #0x0       
	mov ch,[bp-2]  
	mov cl, [bp-6]
	mov ah, #0x2 
	mov dh,[bp-4]  
	int #0x13
	add sp,#6
	pop bp
	ret	

_readChar:	
	push bp
	mov ah,#0
	int 0x16
	pop bp
	ret

_printChar:
	push bp
	mov bp,sp
	mov al, [bp+4]
	mov ah, #0x0e
	int #0x10
	pop bp
	ret

;int interrupt (int number, int AX, int BX, int CX, int DX)
_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]	;get the interrupt number in AL
	push ds		;use self-modifying code to call the right interrupt
	mov bx,cs
	mov ds,bx
	mov si,#intr
	mov [si+1],al	;change the 00 below to the contents of AL
	pop ds
	mov ax,[bp+6]	;get the other parameters AX, BX, CX, and DX
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr:	
	int #0x00	;call the interrupt (00 will be changed above)
	mov ah,#0	;we only want AL returned
	pop bp
	ret

;void putInMemory (int segment, int address, char character)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret

;void makeInterrupt21()
;this sets up the interrupt 0x21 vector
;when an interrupt 0x21 is called in the future,
;_interrupt21ServiceRoutine will run

_makeInterrupt21:
	;get the address of the service routine
	mov dx,#_interrupt21ServiceRoutine
	push ds
	mov ax, #0 ;interrupts are in lowest memory
	mov ds,ax
	mov si,#0x84 ;interrupt 0x21 vector (21 * 4 = 84)
	mov ax,cs	;have interrupt go to the current segment
	mov [si+2],ax
	mov [si],dx	;set up our vector
	pop ds
	ret

_interrupt21ServiceRoutine:
	cmp ax, #0
	je _callPrintString
	cmp ax, #1
	je _callReadString
	cmp ax, #2
	je _callReadSector
	cmp ax, #3
	je _callReadFile
	jmp _end

_end:
	iret

_callReadFile:
	push cx
	push bx
	call _readFile
	add sp, #4
	jmp _end

_callPrintString:
	push bx
	call _printString
	add sp, #2
	jmp _end

_callReadString:
	push bx
	call _readString
	add sp, #2
	jmp _end

_callReadSector:
	push cx
	push bx
	call _readSector
	add sp, #4
	jmp _end

_loadProgram:
	mov ax, #0x2000
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov ax, #0xfff0 ;let's have the stack start at 0x2000:fff0
	mov sp, ax
	mov bp, ax  ; Read the program from the floppy
	mov cl, #12 ;cl holds sector number
	mov dh, #0 ;dh holds head number - 0
	mov ch, #0 ;ch holds track number - 0
	mov ah, #2 ;absolute disk read
	mov al, #1 ;read 1 sector
	mov dl, #0 ;read from floppy disk A
	mov bx, #0 ;read into offset 0 (in the segment)
	int #0x13 ;call BIOS disk read function
	jmp #0x2000:#0 ; Switch to program
