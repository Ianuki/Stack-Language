.intel_syntax noprefix

.section .text
.global _start

_start:
    sub rsp, 1
    
    # GENERATED ASSEMBLY START
    mov rax, 255
    push rax
loop:
    mov rsi, 8
    call log
    mov rax, 1
    push rax
    pop rbx
    pop rax
    sub al, bl
    movzx rax, al
    push rax
    cmp byte ptr [rsp], 0
    jne loop
    # GENERATED ASSEMBLY END

    mov rax, 60
    xor rdi, rdi 
    syscall

log:
    push rcx
    push r11

    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    mov rsi, rsp
    add rsi, 24

    syscall

    pop r11
    pop rcx
    ret
