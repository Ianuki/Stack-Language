    mov rax, 255
    push rax
loop:
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