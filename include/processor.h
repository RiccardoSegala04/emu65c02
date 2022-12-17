#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include <common.h>
#include <stdbool.h>
#include <mem.h>

enum opcode_e {
    ADC_IMM =   0x69,
    ADC_ABS =   0x6D,
    ADC_ZPG =   0x65,
    ADC_IND_X = 0x61,
    ADC_IND_Y = 0x71,
    ADC_ZPG_X = 0x75,
    ADC_ABS_X = 0x7D,
    ADC_ABS_Y = 0x79,

    AND_IMM =   0x29,
    AND_ABS =   0x2D,
    AND_ZPG =   0x25,
    AND_IND_X = 0x21,
    AND_IND_Y = 0x31,
    AND_ZPG_X = 0x35,
    AND_ABS_X = 0x3D,
    AND_ABS_Y = 0x39,

    ASL_ACC =   0x0A,
    ASL_ZPG =   0x06,
    ASL_ZPG_X = 0x16,
    ASL_ABS =   0x0E,
    ASL_ABS_X = 0x1E,

    BCC_REL =   0x90,

    BCS_REL =   0xB0,

    BEQ_REL =   0xF0,

    BIT_ZPG =   0x24,
    BIT_ABS =   0x2C,

    BMI_REL =   0x30,

    BNE_REL =   0xD0,

    BPL_REL =   0x10,

    BRK_IMP =   0x00,

    BVC_REL =   0x50,

    BVS_REL =   0x70,

    CLC_IMP =   0x18,

    CLD_IMP =   0xD8,

    CLI_IMP =   0x58,

    CLV_IMP =   0xB8,

    CMP_IMM =   0xC9,
    CMP_ZPG =   0xC5,
    CMP_ZPG_X = 0xD5,
    CMP_ABS =   0xCD,
    CMP_ABS_X = 0xDD,
    CMP_ABS_Y = 0xD9,
    CMP_IND_X = 0xC1,
    CMP_IND_Y = 0xD1,

    CPX_IMM =   0xE0,
    CPX_ZPG =   0xE4,
    CPX_ABS =   0xEC,

    CPY_IMM =   0xC0,
    CPY_ZPG =   0xC4,
    CPY_ABS =   0xCC,

    DEC_ZPG =   0xC6,
    DEC_ZPG_X = 0xD6,
    DEC_ABS =   0xCE,
    DEC_ABS_X = 0xDE,

    DEX_IMP =   0xCA,

    DEY_IMP =   0x88,

    EOR_IMM =   0x49,
    EOR_ZPG =   0x45,
    EOR_ZPG_X = 0x55,
    EOR_ABS =   0x4D,
    EOR_ABS_X = 0x5D,
    EOR_ABS_Y = 0x59,
    EOR_IND_X = 0x41,
    EOR_IND_Y = 0x51,

    INC_ZPG =   0xE6,
    INC_ZPG_X = 0xF6,
    INC_ABS =   0xEE,
    INC_ABS_X = 0xFE,

    INX_IMP =   0xE8,

    INY_IMP =   0xC8,

    JMP_ABS =   0x4C,
    JMP_IND =   0x6C,

    JSR_ABS =   0x20,

    LDA_IMM =   0xA9,
    LDA_ABS =   0xAD,
    LDA_ZPG =   0xA5,
    LDA_IND_X = 0xA1,
    LDA_IND_Y = 0xB1,
    LDA_ZPG_X = 0xB5,
    LDA_ABS_X = 0xBD,
    LDA_ABS_Y = 0xB9,

    LDX_IMM =   0xA2,
    LDX_ZPG =   0xA6,
    LDX_ZPG_Y = 0xB6,
    LDX_ABS =   0xAE,
    LDX_ABS_Y = 0xBE,

    LDY_IMM =   0xA0,
    LDY_ZPG =   0xA4,
    LDY_ZPG_X = 0xB4,
    LDY_ABS =   0xAC,
    LDY_ABS_X = 0xBC,

    LSR_ACC =   0x4A,
    LSR_ZPG =   0x46,
    LSR_ZPG_X = 0x56,
    LSR_ABS =   0x4E,
    LSR_ABS_X = 0x5E,

    NOP =       0xEA,

    ORA_IMM =   0x09,
    ORA_ZPG =   0x05,
    ORA_ZPG_X = 0x15,
    ORA_ABS =   0x0D,
    ORA_ABS_X = 0x1D,
    ORA_ABS_Y = 0x19,
    ORA_IND_X = 0x01,
    ORA_IND_Y = 0x11,

    PHA_IMP =   0x48,

    PHP_IMP =   0x08,

    PLA_IMP =   0x68,

    PLP_IMP =   0x28,

    ROL_ACC =   0x2A,
    ROL_ZPG =   0x26,
    ROL_ZPG_X = 0x36,
    ROL_ABS =   0x2E,
    ROL_ABS_X = 0x3E,

    ROR_ACC =   0x6A,
    ROR_ZPG =   0x66,
    ROR_ZPG_X = 0x76,
    ROR_ABS =   0x6E,
    ROR_ABS_X = 0x7E,

    RTI_IMP =   0x40,

    RTS_IMP =   0x60,

    SBC_IMM =   0xE9,
    SBC_ZPG =   0xE5,
    SBC_ZPG_X = 0xF5,
    SBC_ABS =   0xED,
    SBC_ABS_X = 0xFD,
    SBC_ABS_Y = 0xF9,
    SBC_IND_X = 0xE1,
    SBC_IND_Y = 0xF1,

    SEC_IMP =   0x38,

    SED_IMP =   0xF8,

    SEI_IMP =   0x78,

    STA_ABS =   0x8D,
    STA_ZPG =   0x85,
    STA_IND_X = 0x81,
    STA_IND_Y = 0x91,
    STA_ZPG_X = 0x95,
    STA_ABS_X = 0x9D,
    STA_ABS_Y = 0x99,

    STX_ZPG =   0x86,
    STX_ZPG_Y = 0x96,
    STX_ABS =   0x8E,

    STY_ZPG =   0x84,
    STY_ZPG_X = 0x94,
    STY_ABS =   0x8C,

    TAX_IMP =   0xAA,

    TAY_IMP =   0xA8,

    TSX_IMP =   0xBA,

    TXA_IMP =   0x8A,

    TXS_IMP =   0x9A,

    TYA_IMP =   0x98,

};

enum processor_op_type_e {
    OP_ACC,
    OP_ABS,
    OP_ABS_X,
    OP_ABS_Y,
    OP_IMM,
    OP_IMP,
    OP_IND,
    OP_X_IND,
    OP_IND_Y,
    OP_REL,
    OP_ZPG,
    OP_ZPG_X,
    OP_ZPG_Y,
    OP_NONE,
};

struct processor_t  {

    bool is_running;

    uint8_t A;
    uint8_t Y;
    uint8_t X;
    uint16_t PC;
    uint8_t SP;

    uint8_t neg, over, brk, dec, ids, zero, carry;
    
    bool A_st;
    bool Y_st;
    bool X_st;
    bool SP_st;
    bool PC_st;

    bool neg_st, over_st, brk_st, dec_st, ids_st, zero_st, carry_st;

    bool button_pressed;
};

typedef void (*op_func)(struct processor_t*, struct mem*);

struct cpu_op_handler_t {
    enum opcode_e op;
    op_func operation;
};

void cpu_init(struct processor_t *cpu);
void cpu_load_res_addr(struct processor_t* cpu, struct mem* mem);
void cpu_print_debug(struct processor_t *cpu);

uint8_t cpu_fetch(struct processor_t *cpu, struct mem* m);
uint8_t cpu_get_operand_byte(struct processor_t *cpu, struct mem* m);
uint16_t cpu_get_operand_short(struct processor_t *cpu, struct mem* m);

int cpu_step(struct processor_t *cpu, struct mem* mem, enum opcode_e op);

int cpu_op_get_n_bytes(enum opcode_e op);
enum processor_op_type_e cpu_get_op_type(enum opcode_e op);
const char* cpu_get_op_name(enum opcode_e op);

#endif
