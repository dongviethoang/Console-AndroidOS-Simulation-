; Stage2: loads a small kernel from disk (LBA_KERN) and jumps to it.
; Assemble: nasm -f bin stage2.asm -o stage2.bin
org 0x8000

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    sti

    mov si, s_msg
    call print_str
    mov si, launch_msg
    call print_str

    ; make DS point to this code/data segment so DS:SI can reference dap
    push cs
    pop ax
    mov ds, ax
    mov si, dap           ; DS:SI -> DAP

    ; ensure DL still contains boot drive (do not clobber it here)
    mov ah, 0x42          ; Extended Read
    int 0x13
    jc disk_fail

    ; on success, jump to kernel loaded at 0x0000:KERNEL_OFF
    jmp 0x0000:KERNEL_OFF

disk_fail:
    ; AH holds status; print hex then message
    mov al, ah
    call print_hex_byte
    mov si, err_disk
    call print_str
    jmp $

; print routine
print_str:
    pusha
.p_loop:
    lodsb
    cmp al, 0
    je .p_done
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    jmp .p_loop
.p_done:
    popa
    ret

; print_hex_byte: AL = byte -> print two hex chars (upper then lower)
print_hex_byte:
    pusha
    mov bl, al
    shr al, 4
    and al, 0x0F
    cmp al, 9
    jbe .h1
    add al, 7
.h1:
    add al, '0'
    mov ah, 0x0E
    int 0x10

    mov al, bl
    and al, 0x0F
    cmp al, 9
    jbe .h2
    add al, 7
.h2:
    add al, '0'
    mov ah, 0x0E
    int 0x10
    popa
    ret

s_msg      db "Stage2: loader running.",0x0D,0x0A,0
launch_msg db "Loading kernel...",0x0D,0x0A,0
err_disk   db 0x0D,0x0A, "Disk read error while loading kernel. Halting.",0x0D,0x0A,0

KERNEL_OFF   equ 0x9000
KERNEL_SECTS equ 16
KERNEL_LBA   equ 9

; Disk Address Packet (16 bytes)
dap:
    db 0x10                ; size
    db 0x00                ; reserved
    dw KERNEL_SECTS        ; sectors to read
    dw KERNEL_OFF          ; offset
    dw 0x0000              ; segment
    dq KERNEL_LBA          ; starting LBA (qword)

times 0x2000-($-$$) db 0