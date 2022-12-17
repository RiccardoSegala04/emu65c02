    .org $8000

; Code section
reset:
end:
    jmp end

    .org $fffc
    .word reset
    .word $0000
