; ============================================
; BASIC ARITHMETIC TEST
; Tests: MOV, ADD, SUB, MUL, DIV
; ============================================

_start:
    ; Initialize registers
    MOV RAX, 10      ; RAX = 10
    MOV RBX, 5       ; RBX = 5
    MOV RCX, 3       ; RCX = 3
    MOV RDX, 2       ; RDX = 2
    
    ; Test ADD
    ADD RAX, RBX     ; RAX = 15 (10 + 5)
    ADD RCX, RDX     ; RCX = 5 (3 + 2)
    
    ; Test SUB
    SUB RAX, RCX     ; RAX = 10 (15 - 5)
    SUB RBX, RDX     ; RBX = 3 (5 - 2)
    
    ; Test MUL (RAX * RBX, result in RDX:RAX)
    MOV RAX, 4
    MOV RBX, 6
    MUL RBX          ; RAX = 24 (4 * 6)
    
    ; Test DIV (RDX:RAX / RCX, quotient in RAX)
    MOV RAX, 20
    MOV RDX, 0       ; Clear upper 64 bits
    MOV RCX, 4
    DIV RCX          ; RAX = 5 (20 / 4)
    
    ; Store results in memory
    MOV [0x100], RAX ; Store 5 at address 0x100
    MOV [0x108], RBX ; Store 3 at address 0x108
    MOV [0x110], RCX ; Store 4 at address 0x110
    
    HLT
