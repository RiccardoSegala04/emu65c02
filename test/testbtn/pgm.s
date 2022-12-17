; Constants
DDRB        = $4003
PTB         = $4001
PTB_MASK    = %11101111
BTN_MASK    = %00010000
STACK_ADDR  = $ff
COUNT_ADDR  = $00

; Code section
    .org $8000

inc_led:
    
    inc COUNT_ADDR
    lda COUNT_ADDR

    cmp #$f
    bne eif_res

if_res:
    lda #0
    sta COUNT_ADDR

eif_res:
    sta PTB

    rts


reset:

    ldx #STACK_ADDR
    txs

    lda #0
    sta COUNT_ADDR

    lda #PTB_MASK
    sta DDRB

inc_loop:

    lda PTB
    and #BTN_MASK

    cmp #BTN_MASK
    bne inc_loop

    jsr inc_led

rel_loop:

    lda PTB
    and #BTN_MASK

    cmp #0
    bne rel_loop

    jmp inc_loop

; Reset vector
    .org $fffc
    .word reset
    .word $0000


