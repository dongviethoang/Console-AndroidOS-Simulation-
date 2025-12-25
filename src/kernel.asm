; Small demo kernel loaded at 0x0000:0x9000
; Assemble: nasm -f bin kernel.asm -o kernel.bin
org 0x9000

; define kernel size in sectors for padding
KERNEL_SECTS equ 16

start_k:
    cli
    xor ax, ax
    mov ds, ax
    sti

    mov si, k_title
    call print_str

    mov si, menu_msg
    call print_str

wait_key:
    xor ah, ah
    int 0x16            ; wait key
    mov ah, 0x0E
    mov bh, 0
    mov bl, 0x07
    int 0x10            ; echo pressed key
    jmp wait_key

; print routine
print_str:
    pusha
.p_l:
    lodsb
    cmp al, 0
    je .p_done
    mov ah, 0x0E
    mov bh, 0
    mov bl, 0x07
    int 0x10
    jmp .p_l
.p_done:
    popa
    ret

k_title  db "Console AndroidOS (demo kernel) loaded.",0x0D,0x0A,0
menu_msg db "Demo kernel: press any key to show key (loop).",0x0D,0x0A,0

; pad to KERNEL_SECTS sectors (constant now defined)
times (512 * KERNEL_SECTS) - ($ - $$) db 0   ; filepath: d:/Console AndroidOS/src/kernel.asm