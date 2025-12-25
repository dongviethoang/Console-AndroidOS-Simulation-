; Boot sector (512 bytes) that loads a second-stage binary via INT13h (Extended) DAP
; Assembles with: nasm -f bin bootloader.asm -o bootloader.bin
org 0x7C00

    cli
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; store drive (DL provided by BIOS) for later use
    mov [drive], dl

    ; show a brief banner
    mov si, vendor
    call print_str
    mov si, title
    call print_str
    mov si, build
    call print_str

    ; attempt to read stage2 (LBA 1) using INT 13h ext function 0x42
    ; DAP is defined below (16 bytes). We load N_SECTORS sectors to 0x0000:LOAD_OFF
    ; set ES:BX -> address of dap (DAP is in same segment)
    xor ax, ax
    mov bx, dap           ; immediate offset of dap
    mov es, ax            ; load ES from AX (can't mov es, imm)
    ; restore boot drive into DL
    mov dl, [drive]
    mov ah, 0x42          ; INT13h ext read
    int 0x13
    sti
    jc disk_fail

    ; jump to loaded stage2 at segment 0x0000, offset LOAD_OFF
    jmp 0x0000:LOAD_OFF

disk_fail:
    mov si, err_disk
    call print_str
    jmp $

; helper to print zero-terminated string (BIOS teletype)
print_str:
    pusha
.print_loop:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    jmp .print_loop
.done:
    popa
    ret

; data
vendor      db "Boot: Beta Corporation (c).",0x0D,0x0A,0
title       db "Console AndroidOS boot",0x0D,0x0A,0
build       db "Bootloader v1.0",0x0D,0x0A,0
err_disk    db "Disk read error. Halting.",0x0D,0x0A,0

drive       db 0

; DAP (Disk Address Packet) for INT13h AH=0x42
; size (1), reserved (1), sectors (word), offset (word), segment (word), starting LBA (qword)
; We'll load 8 sectors starting at LBA 1 into 0x0000:LOAD_OFF
DAP_SIZE    equ 16
LOAD_SECTORS equ 8
LOAD_OFF    equ 0x8000

dap:
    db 0x10                ; size of packet (16)
    db 0x00                ; reserved
    dw LOAD_SECTORS        ; number of sectors to read
    dw LOAD_OFF            ; offset
    dw 0x0000              ; segment
    dq 1                   ; starting LBA (read from LBA 1)

times 510-($-$$) db 0
dw 0xAA55