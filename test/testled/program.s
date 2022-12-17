; 65c02 Program temeplate

DDRA    = $4002
DDRB    = $4003
PTA     = $4000
PTB     = $4001

    .org $8000

main:

    LDA #$f
    STA DDRB

    LDA #0
loop:

    STA PTB
    LDA PTB

    CMP #15
    BNE endif
    LDA #0
    jmp loop
    
endif:
    CLC
    ADC #1

    jmp loop

end: jmp end

    .org $fffc
    .word main
    .word $0000

