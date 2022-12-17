; 65c02 Program temeplate

DDRA    = $4002
DDRB    = $4003
PTA     = $4000
PTB     = $4001

.org $8000

main:

    LDA #7
    STA DDRB

    LDA #1
    STA PTB

end: jmp END

.org $fffc
.word main
.word $0000
