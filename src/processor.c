#include <processor.h>
#include <string.h>
#include <stdio.h>
#include <log.h>

/*---------------------------------------------------*/
/* brief: init the cpu struct */
/*---------------------------------------*/
void cpu_init(struct processor_t* cpu) {
    memset(cpu, 0, sizeof(*cpu));
    cpu->is_running = true;
    cpu->PC = MEM_RES;
}

/*----------------------------------------------------------------------*/
/* brief: load the address of the program from the resect vector */
/*----------------------------------------------------------------*/
void cpu_load_res_addr(struct processor_t* cpu, struct mem* mem) {
    cpu->PC = mem->data[MEM_RES] | (mem->data[MEM_RES+1]<<8);
}

/*---------------------------------------------------*/
/* brief: prints the cpu state */
/*---------------------------------------*/
void cpu_print_debug(struct processor_t* c) {
    printf("Accumulator: 0x%02x\n", c->A & 0xff);
    printf("Y register: 0x%02x\n", c->Y & 0xff);
    printf("X register: 0x%02x\n", c->X & 0xff);
    printf("Stack pointer: 0x%02x\n", c->SP & 0xff);
    printf("Program counter: 0x%02x\n", c->PC & 0xff);
    printf("Condition code: %d%d%d%d%d%d%d%d\n", 
        c->neg, c->over, 0, c->brk, 
        c->dec, c->ids, c->zero, c->carry
    );
}

/*--------------------------------------------------------------*/
/* brief: return the type of an instruction */
/*---------------------------------------*/
enum processor_op_type_e cpu_get_op_type(enum opcode_e op) {
    switch(op) {

        case ASL_ACC:
        case LSR_ACC:
        case ROL_ACC:
        case ROR_ACC:
            return OP_ACC;

        case ADC_ABS:
        case AND_ABS:
        case ASL_ABS:
        case BIT_ABS:
        case CMP_ABS:
        case CPX_ABS:
        case CPY_ABS:
        case DEC_ABS:
        case EOR_ABS:
        case INC_ABS:
        case JMP_ABS:
        case JSR_ABS:
        case LDA_ABS:
        case LDX_ABS:
        case LDY_ABS:
        case LSR_ABS:
        case ORA_ABS:
        case ROL_ABS:
        case ROR_ABS:
        case SBC_ABS:
        case STA_ABS:
        case STX_ABS:
        case STY_ABS:
            return OP_ABS;

        case ADC_ABS_X:
        case AND_ABS_X:
        case ASL_ABS_X:
        case CMP_ABS_X:
        case DEC_ABS_X:
        case EOR_ABS_X:
        case INC_ABS_X:
        case LDA_ABS_X:
        case LDY_ABS_X:
        case LSR_ABS_X:
        case ORA_ABS_X:
        case ROL_ABS_X:
        case ROR_ABS_X:
        case SBC_ABS_X:
        case STA_ABS_X:
            return OP_ABS_X;

        case ADC_ABS_Y:
        case AND_ABS_Y:
        case CMP_ABS_Y:
        case EOR_ABS_Y:
        case LDA_ABS_Y:
        case LDX_ABS_Y:
        case ORA_ABS_Y:
        case SBC_ABS_Y: 
        case STA_ABS_Y:
            return OP_ABS_Y;

        case ADC_IMM:
        case AND_IMM:
        case CPX_IMM:
        case CMP_IMM:
        case CPY_IMM:
        case EOR_IMM:
        case LDA_IMM:
        case LDX_IMM:
        case LDY_IMM:
        case ORA_IMM:
        case SBC_IMM:
        case NOP:
            return OP_IMM;

        case BRK_IMP:
        case CLC_IMP:
        case CLD_IMP:
        case CLI_IMP:
        case CLV_IMP:
        case DEX_IMP:
        case DEY_IMP:
        case INX_IMP:
        case INY_IMP:
        case PHA_IMP:
        case PHP_IMP:
        case PLA_IMP:
        case PLP_IMP:
        case RTI_IMP:
        case RTS_IMP:
        case SEC_IMP:
        case SED_IMP:
        case SEI_IMP:
        case TAX_IMP:
        case TAY_IMP:
        case TSX_IMP:
        case TXA_IMP:
        case TXS_IMP:
        case TYA_IMP:
            return OP_IMP;

        case JMP_IND:
            return OP_IND;

        case ADC_IND_X:
        case AND_IND_X:
        case CMP_IND_X:
        case EOR_IND_X:
        case LDA_IND_X:
        case ORA_IND_X:
        case SBC_IND_X:
        case STA_IND_X:
            return OP_X_IND;

        case ADC_IND_Y:
        case AND_IND_Y:
        case CMP_IND_Y:
        case EOR_IND_Y:
        case LDA_IND_Y:
        case ORA_IND_Y:
        case SBC_IND_Y:
        case STA_IND_Y:
            return OP_IND_Y;

        case BCC_REL:
        case BCS_REL:
        case BEQ_REL:
        case BMI_REL:
        case BNE_REL:
        case BPL_REL:
        case BVC_REL:
        case BVS_REL:
            return OP_REL;

        case ADC_ZPG:
        case AND_ZPG:
        case ASL_ZPG:
        case BIT_ZPG:
        case CMP_ZPG:
        case CPX_ZPG:
        case CPY_ZPG:
        case DEC_ZPG:
        case EOR_ZPG:
        case INC_ZPG:
        case LDA_ZPG:
        case LDX_ZPG:
        case LDY_ZPG:
        case LSR_ZPG:
        case ORA_ZPG:
        case ROL_ZPG:
        case ROR_ZPG:
        case SBC_ZPG:
        case STA_ZPG:
        case STX_ZPG:
        case STY_ZPG:
            return OP_ZPG;

        case ADC_ZPG_X:
        case AND_ZPG_X:
        case ASL_ZPG_X:
        case CMP_ZPG_X:
        case DEC_ZPG_X:
        case EOR_ZPG_X:
        case INC_ZPG_X:
        case LDA_ZPG_X:
        case LDY_ZPG_X:
        case LSR_ZPG_X:
        case ORA_ZPG_X:
        case ROL_ZPG_X:
        case ROR_ZPG_X:
        case SBC_ZPG_X:
        case STA_ZPG_X:
        case STY_ZPG_X:
            return OP_ZPG_X;

        case LDX_ZPG_Y:
        case STX_ZPG_Y:
            return OP_ZPG_Y;
    }

    return OP_NONE;
}

/*--------------------------------------------------------------*/
/* brief: return the number of bytes for an instruction */
/*---------------------------------------*/
int cpu_op_get_n_bytes(enum opcode_e op) {
    switch(op) {
        case ADC_IMM:
        case ADC_ZPG:
        case ADC_IND_X:
        case ADC_IND_Y:
        case ADC_ZPG_X:
        case AND_IMM:
        case AND_ZPG:
        case AND_IND_X:
        case AND_IND_Y:
        case AND_ZPG_X:
        case ASL_ZPG:
        case ASL_ZPG_X:
        case BCC_REL:
        case BCS_REL:
        case BEQ_REL:
        case BIT_ZPG:
        case BMI_REL:
        case BNE_REL:
        case BPL_REL:
        case BVC_REL:
        case BVS_REL:
        case CMP_IMM:
        case CMP_ZPG:
        case CMP_ZPG_X:
        case CMP_IND_X:
        case CMP_IND_Y:
        case CPX_IMM:
        case CPX_ZPG:
        case CPY_IMM:
        case CPY_ZPG:
        case DEC_ZPG:
        case DEC_ZPG_X:
        case EOR_IMM:
        case EOR_ZPG:
        case EOR_ZPG_X:
        case EOR_IND_X:
        case EOR_IND_Y:
        case INC_ZPG:
        case INC_ZPG_X:
        case JMP_IND:
        case LDA_IMM:
        case LDA_ZPG:
        case LDA_IND_X:
        case LDA_IND_Y:
        case LDA_ZPG_X:
        case LDX_IMM:
        case LDX_ZPG:
        case LDX_ZPG_Y:
        case LDY_IMM:
        case LDY_ZPG:
        case LDY_ZPG_X:
        case LSR_ZPG:
        case LSR_ZPG_X:
        case ORA_IMM:
        case ORA_ZPG:
        case ORA_ZPG_X:
        case ORA_IND_X:
        case ORA_IND_Y:
        case ROL_ZPG:
        case ROL_ZPG_X:
        case ROR_ZPG:
        case ROR_ZPG_X:
        case SBC_IMM:
        case SBC_ZPG:
        case SBC_ZPG_X:
        case SBC_IND_X:
        case SBC_IND_Y:
        case STA_ZPG:
        case STA_IND_X:
        case STA_IND_Y:
        case STA_ZPG_X:
        case STX_ZPG:
        case STX_ZPG_Y:
        case STY_ZPG:
        case STY_ZPG_X:
            return 1;

        case ADC_ABS:
        case ADC_ABS_X:
        case ADC_ABS_Y:
        case AND_ABS:
        case AND_ABS_X:
        case AND_ABS_Y:
        case ASL_ABS:
        case ASL_ABS_X:
        case BIT_ABS:
        case CMP_ABS:
        case CMP_ABS_X:
        case CMP_ABS_Y:
        case CPX_ABS:
        case CPY_ABS:
        case DEC_ABS:
        case DEC_ABS_X:
        case EOR_ABS:
        case EOR_ABS_X:
        case EOR_ABS_Y:
        case INC_ABS:
        case INC_ABS_X:
        case JMP_ABS:
        case JSR_ABS:
        case LDA_ABS:
        case LDA_ABS_X:
        case LDA_ABS_Y:
        case LDX_ABS:
        case LDX_ABS_Y:
        case LDY_ABS_X:
        case LSR_ABS:
        case LSR_ABS_X:
        case ORA_ABS:
        case ORA_ABS_X:
        case ORA_ABS_Y:
        case ROL_ABS:
        case ROL_ABS_X:
        case ROR_ABS:
        case ROR_ABS_X:
        case SBC_ABS:
        case SBC_ABS_X:
        case SBC_ABS_Y:
        case STA_ABS:
        case STA_ABS_X:
        case STA_ABS_Y:
        case STX_ABS:
        case STY_ABS:
            return 2;
        default:
            break;
    }

    return 0;
}

const char* cpu_get_op_name(enum opcode_e op) {

    switch(op) {
        case ADC_IMM:
        case ADC_ABS:
        case ADC_ZPG:
        case ADC_IND_X:
        case ADC_IND_Y:
        case ADC_ZPG_X:
        case ADC_ABS_X:
        case ADC_ABS_Y:
            return "ADC";
        case AND_IMM:
        case AND_ABS:
        case AND_ZPG:
        case AND_IND_X:
        case AND_IND_Y:
        case AND_ZPG_X:
        case AND_ABS_X:
        case AND_ABS_Y:
            return "AND";

        case ASL_ACC:
        case ASL_ZPG:
        case ASL_ZPG_X:
        case ASL_ABS:
        case ASL_ABS_X:
            return "ASL";

        case BCC_REL:
            return "BCC";

        case BCS_REL:
            return "BCS";

        case BEQ_REL:
            return "BEQ";

        case BIT_ZPG:
        case BIT_ABS:
            return "BIT";

        case BMI_REL:
            return "BMI";

        case BNE_REL:
            return "BNE"; 

        case BPL_REL:
            return "BPL";

        case BRK_IMP:
            return "BRK";

        case BVC_REL:
            return "BVC";

        case BVS_REL:
            return "BVS";

        case CLC_IMP:
            return "CLC";

        case CLD_IMP:
            return "CLD";

        case CLI_IMP:
            return "CLI";

        case CLV_IMP:
            return "CLV";

        case CMP_IMM:
        case CMP_ZPG:
        case CMP_ZPG_X:
        case CMP_ABS:
        case CMP_ABS_X:
        case CMP_ABS_Y:
        case CMP_IND_X:
        case CMP_IND_Y:
            return "CMP";

        case CPX_IMM:
        case CPX_ZPG:
        case CPX_ABS:
            return "CPX";

        case CPY_IMM:
        case CPY_ZPG:
        case CPY_ABS:
            return "CPY";

        case DEC_ZPG:
        case DEC_ZPG_X:
        case DEC_ABS:
        case DEC_ABS_X:
            return "DEC";

        case DEX_IMP:
            return "DEX";

        case DEY_IMP:
            return "DEY";

        case EOR_IMM:
        case EOR_ZPG:
        case EOR_ZPG_X:
        case EOR_ABS:
        case EOR_ABS_X:
        case EOR_ABS_Y:
        case EOR_IND_X:
        case EOR_IND_Y:
            return "EOR";

        case INC_ZPG:
        case INC_ZPG_X:
        case INC_ABS:
        case INC_ABS_X:
            return "INC";

        case INX_IMP:
            return "INX";

        case INY_IMP:
            return "INY";

        case JMP_ABS:
        case JMP_IND:
            return "JMP";

        case JSR_ABS:
            return "JSR";

        case LDA_IMM:
        case LDA_ABS:
        case LDA_ZPG:
        case LDA_IND_X:
        case LDA_IND_Y:
        case LDA_ZPG_X:
        case LDA_ABS_X:
        case LDA_ABS_Y:
            return "LDA";

        case LDX_IMM:
        case LDX_ZPG:
        case LDX_ZPG_Y:
        case LDX_ABS:
        case LDX_ABS_Y:
            return "LDX";

        case LDY_IMM:
        case LDY_ZPG:
        case LDY_ZPG_X:
        case LDY_ABS:
        case LDY_ABS_X:
            return "LDY";

        case LSR_ACC:
        case LSR_ZPG:
        case LSR_ZPG_X:
        case LSR_ABS:
        case LSR_ABS_X:
            return "LSR";

        case NOP:
            return "NOP";

        case ORA_IMM:
        case ORA_ZPG:
        case ORA_ZPG_X:
        case ORA_ABS:
        case ORA_ABS_X:
        case ORA_ABS_Y:
        case ORA_IND_X:
        case ORA_IND_Y:
            return "ORA";

        case PHA_IMP:
            return "PHA";

        case PHP_IMP:
            return "PHP";

        case PLA_IMP:
            return "PLA";

        case PLP_IMP:
            return "PLP";

        case ROL_ACC:
        case ROL_ZPG:
        case ROL_ZPG_X:
        case ROL_ABS:
        case ROL_ABS_X:
            return "ROL";

        case ROR_ACC:
        case ROR_ZPG:
        case ROR_ZPG_X:
        case ROR_ABS:
        case ROR_ABS_X:
            return "ROR";

        case RTI_IMP:
            return "RTI";

        case RTS_IMP:
            return "RTS";

        case SBC_IMM:
        case SBC_ZPG:
        case SBC_ZPG_X:
        case SBC_ABS:
        case SBC_ABS_X:
        case SBC_ABS_Y:
        case SBC_IND_X:
        case SBC_IND_Y:
            return "SBC";

        case SEC_IMP:
            return "SEC";

        case SED_IMP:
            return "SED";

        case SEI_IMP:
            return "SEI";

        case STA_ABS:
        case STA_ZPG:
        case STA_IND_X:
        case STA_IND_Y:
        case STA_ZPG_X:
        case STA_ABS_X:
        case STA_ABS_Y:
            return "STA";

        case STX_ZPG:
        case STX_ZPG_Y:
        case STX_ABS:
            return "STX";

        case STY_ZPG:
        case STY_ZPG_X:
        case STY_ABS:
            return "STY";

        case TAX_IMP:
            return "TAX";

        case TAY_IMP:
            return "TAY";

        case TSX_IMP:
            return "TSX";

        case TXA_IMP:
            return "TXA";

        case TXS_IMP:
            return "TSX";

        case TYA_IMP:
            return "TYA";
    }

    return NULL;
}

/*---------------------------------------------------*/
/* brief: return the next op code */
/*---------------------------------------*/
uint8_t cpu_fetch(struct processor_t* cpu, struct mem* m) {
    return cpu_get_operand_byte(cpu, m);
}

/*---------------------------------------------------*/
/* brief: return the next operand byte */
/*---------------------------------------*/
uint8_t cpu_get_operand_byte(struct processor_t* cpu, struct mem* m) {
    return m->data[cpu->PC++];
}

/*---------------------------------------------------*/
/* brief: return the next operand short */
/*---------------------------------------*/
uint16_t cpu_get_operand_short(struct processor_t* cpu, struct mem* m) {
    uint16_t param = cpu_get_operand_byte(cpu, m);

    param = param | (cpu_get_operand_byte(cpu, m)<<8);

    m->last_selected = param;

    return param;
}

/*---------------------------------------------------*/
/* brief: return zpg address */
/*---------------------------------------*/
uint8_t cpu_get_address_zpg(struct processor_t* cpu, struct mem* m) {
    uint8_t address = cpu_get_operand_byte(cpu, m);
    m->last_selected = address;
    return address;
}

/*---------------------------------------------------*/
/* brief: return ind x address */
/*---------------------------------------*/
uint16_t cpu_get_address_ind_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_byte(cpu, mem) + cpu->X;

    address = mem_get_data_short(mem, address);

    mem->last_selected = address;

    return address;
}

/*---------------------------------------------------*/
/* brief: return ind y address */
/*---------------------------------------*/
uint16_t cpu_get_address_ind_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address;
    address = cpu_get_operand_byte(cpu, mem);
    address = mem_get_data_byte(mem, address);
    address += cpu->Y;
    mem->last_selected = address;
    return address;
}

/*---------------------------------------------------*/
/* brief: return zpg x address */
/*---------------------------------------*/
uint8_t cpu_get_address_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint8_t address = cpu_get_operand_byte(cpu, mem) + cpu->X + cpu->carry;
    mem->last_selected = address;
    return address;
}

/*---------------------------------------------------*/
/* brief: return zpg y address */
/*---------------------------------------*/
uint8_t cpu_get_address_zpg_y(struct processor_t* cpu, struct mem* mem) {
    uint8_t address = cpu_get_operand_byte(cpu, mem) + cpu->Y + cpu->carry;
    mem->last_selected = address;
    return address;
}


/*---------------------------------------------------*/
/* brief: return abs x address */
/*---------------------------------------*/
uint16_t cpu_get_address_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem) + cpu->X;
    mem->last_selected = address;
    return address;
}

/*---------------------------------------------------*/
/* brief: return abs y address */
/*---------------------------------------*/
uint16_t cpu_get_address_abs_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem) + cpu->Y;
    mem->last_selected = address;
    return address;
}

/*---------------------------------------------------*/
/* operation handlers */

/*---------------------------------------------------*/
/* brief: handle unsupported operation */
/*---------------------------------------*/
static void cpu_handle_unsupported(enum opcode_e op) {
    //printf("Operation not supported yet! 0x%02x\n", op);
}

/*---------------------------------------------------*/
/* ADC OPERATION */

/*---------------------------------------------------*/
/* brief: handle adc imm */
/*---------------------------------------*/
static void cpu_handle_adc_imm(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper;
    oper = cpu_get_operand_byte(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A += oper + cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle adc abs */
/*---------------------------------------*/
static void cpu_handle_adc_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address;
    address = cpu_get_operand_short(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A += mem->data[address] + cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle adc zpg */
/*---------------------------------------*/
static void cpu_handle_adc_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t address;
    address = cpu_get_address_zpg(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A += mem->data[address] + cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle adc ind x */
/*---------------------------------------*/
static void cpu_handle_adc_ind_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_ind_x(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A += mem->data[address];

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle adc ind y */
/*---------------------------------------*/
static void cpu_handle_adc_ind_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_ind_y(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A += mem->data[address];

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle adc zpg x */
/*---------------------------------------*/
static void cpu_handle_adc_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint8_t address = cpu_get_address_ind_y(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A += mem->data[address] + cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle adc abs x */
/*---------------------------------------*/
static void cpu_handle_adc_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_x(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A += mem->data[address] + cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle adc abs y */
/*---------------------------------------*/
static void cpu_handle_adc_abs_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_y(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A += mem->data[address] + cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* AND OPERATION */

/*---------------------------------------------------*/
/* brief: handle and imm */
/*---------------------------------------*/
static void cpu_handle_and_imm(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper;
    oper = cpu_get_operand_byte(cpu, mem);
    cpu->A = cpu->A & oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle and abs */
/*---------------------------------------*/
static void cpu_handle_and_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address;
    address = cpu_get_operand_short(cpu, mem);
    cpu->A = cpu->A & mem->data[address];

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle and zpg */
/*---------------------------------------*/
static void cpu_handle_and_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t address;
    address = cpu_get_address_zpg(cpu, mem);
    cpu->A = cpu->A & mem->data[address];

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle and ind x */
/*---------------------------------------*/
static void cpu_handle_and_ind_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_ind_x(cpu, mem);

    cpu->A = cpu->A & mem->data[address];

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle and ind y */
/*---------------------------------------*/
static void cpu_handle_and_ind_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_ind_y(cpu, mem);

    cpu->A = cpu->A & mem->data[address];

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle and zpg x */
/*---------------------------------------*/
static void cpu_handle_and_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint8_t address = cpu_get_address_zpg_x(cpu, mem);

    cpu->A = cpu->A & mem->data[address];

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle and abs x */
/*---------------------------------------*/
static void cpu_handle_and_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_x(cpu, mem);

    cpu->A = cpu->A & mem->data[address];

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle and abs y */
/*---------------------------------------*/
static void cpu_handle_and_abs_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_y(cpu, mem);

    cpu->A = cpu->A & mem->data[address];

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* ASL OPERATION */

/*---------------------------------------------------*/
/* brief: handle asl acc */
/*---------------------------------------*/
static void cpu_handle_asl_acc(struct processor_t* cpu, struct mem* mem) {

    cpu->carry = cpu->A>>7;

    cpu->A = cpu->A << 1;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle asl zpg */
/*---------------------------------------*/
static void cpu_handle_asl_zpg(struct processor_t* cpu, struct mem* mem) {

    uint8_t address = cpu_get_address_zpg(cpu, mem);

    cpu->carry = mem->data[address]>>7;

    mem->data[address] = mem->data[address] << 1;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle asl zpg x */
/*---------------------------------------*/
static void cpu_handle_asl_zpg_x(struct processor_t* cpu, struct mem* mem) {

    uint8_t address = cpu_get_address_zpg_x(cpu, mem);

    cpu->carry = mem->data[address]>>7;

    mem->data[address] = mem->data[address] << 1;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle asl abs */
/*---------------------------------------*/
static void cpu_handle_asl_abs(struct processor_t* cpu, struct mem* mem) {

    uint16_t address = cpu_get_operand_short(cpu, mem);

    cpu->carry = mem->data[address]>>7;

    mem->data[address] = mem->data[address] << 1;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle asl abs x */
/*---------------------------------------*/
static void cpu_handle_asl_abs_x(struct processor_t* cpu, struct mem* mem) {

    uint16_t address = cpu_get_address_abs_x(cpu, mem);

    cpu->carry = mem->data[address]>>7;

    mem->data[address] = mem->data[address] << 1;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* BCC OPERATION */

/*---------------------------------------------------*/
/* brief: handle bcc rel */
/*---------------------------------------*/
static void cpu_handle_bcc_rel(struct processor_t* cpu, struct mem* mem) {

    int8_t oper = cpu_get_operand_byte(cpu, mem);

    if(!cpu->carry) {
        cpu->PC += oper;
    }
}

/*---------------------------------------------------*/
/* BCS OPERATION */

/*---------------------------------------------------*/
/* brief: handle bcs rel */
/*---------------------------------------*/
static void cpu_handle_bcs_rel(struct processor_t* cpu, struct mem* mem) {

    int8_t oper = cpu_get_operand_byte(cpu, mem);

    if(cpu->carry) {
        cpu->PC += oper;
    }
}

/*---------------------------------------------------*/
/* BEQ OPERATION */

/*---------------------------------------------------*/
/* brief: handle beq rel */
/*---------------------------------------*/
static void cpu_handle_beq_rel(struct processor_t* cpu, struct mem* mem) {

    int8_t oper = cpu_get_operand_byte(cpu, mem);

    if(cpu->zero) {
        cpu->PC += oper;
    }
}

/*---------------------------------------------------*/
/* BMI OPERATION */

/*---------------------------------------------------*/
/* brief: handle bmi rel */
/*---------------------------------------*/
static void cpu_handle_bmi_rel(struct processor_t* cpu, struct mem* mem) {

    int8_t oper = cpu_get_operand_byte(cpu, mem);

    if(cpu->neg) {
        cpu->PC += oper;
    }
}

/*---------------------------------------------------*/
/* BNE OPERATION */

/*---------------------------------------------------*/
/* brief: handle bne rel */
/*---------------------------------------*/
static void cpu_handle_bne_rel(struct processor_t* cpu, struct mem* mem) {

    int8_t oper = cpu_get_operand_byte(cpu, mem);

    if(!cpu->zero) {
        cpu->PC += oper;
    }
}

/*---------------------------------------------------*/
/* BPL OPERATION */

/*---------------------------------------------------*/
/* brief: handle bpl rel */
/*---------------------------------------*/
static void cpu_handle_bpl_rel(struct processor_t* cpu, struct mem* mem) {

    int8_t oper = cpu_get_operand_byte(cpu, mem);

    if(!cpu->neg) {
        cpu->PC += oper;
    }
}

/*---------------------------------------------------*/
/* BVC OPERATION */

/*---------------------------------------------------*/
/* brief: handle bvc rel */
/*---------------------------------------*/
static void cpu_handle_bvc_rel(struct processor_t* cpu, struct mem* mem) {

    int8_t oper = cpu_get_operand_byte(cpu, mem);

    if(!cpu->over) {
        cpu->PC += oper;
    }
}

/*---------------------------------------------------*/
/* BVS OPERATION */

/*---------------------------------------------------*/
/* brief: handle bvs rel */
/*---------------------------------------*/
static void cpu_handle_bvs_rel(struct processor_t* cpu, struct mem* mem) {

    int8_t oper = cpu_get_operand_byte(cpu, mem);

    if(cpu->over) {
        cpu->PC += oper;
    }
}

/*---------------------------------------------------*/
/* CLC OPERATION */

/*---------------------------------------------------*/
/* brief: handle clc imp */
/*---------------------------------------*/
static void cpu_handle_clc_imp(struct processor_t* cpu, struct mem* mem) {
    cpu->carry = 0;
}

/*---------------------------------------------------*/
/* CLD OPERATION */

/*---------------------------------------------------*/
/* brief: handle cld imp */
/*---------------------------------------*/
static void cpu_handle_cld_imp(struct processor_t* cpu, struct mem* mem) {
    cpu->dec = 0;
}

/*---------------------------------------------------*/
/* CLI OPERATION */

/*---------------------------------------------------*/
/* brief: handle cli imp */
/*---------------------------------------*/
static void cpu_handle_cli_imp(struct processor_t* cpu, struct mem* mem) {
    cpu->ids = 0;
}

/*---------------------------------------------------*/
/* CLV OPERATION */

/*---------------------------------------------------*/
/* brief: handle clv imp */
/*---------------------------------------*/
static void cpu_handle_clv_imp(struct processor_t* cpu, struct mem* mem) {
    cpu->over = 0;
}

/*---------------------------------------------------*/
/* CMP OPERATION */

/*---------------------------------------------------*/
/* brief: handle cmp imm */
/*---------------------------------------*/
static void cpu_handle_cmp_imm(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper;
    oper = cpu_get_operand_byte(cpu, mem);

    cpu->carry = cpu->A>=oper;
    
    oper = cpu->A - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle cmp zpg */
/*---------------------------------------*/
static void cpu_handle_cmp_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper;
    oper = cpu_get_address_zpg(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->A>=oper;
    
    oper = cpu->A - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cmp zpg x */
/*---------------------------------------*/
static void cpu_handle_cmp_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper = cpu_get_address_zpg_x(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->A>=oper;
    
    oper = cpu->A - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cmp abs */
/*---------------------------------------*/
static void cpu_handle_cmp_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_operand_short(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->A>=oper;
    
    oper = cpu->A - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cmp abs x */
/*---------------------------------------*/
static void cpu_handle_cmp_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_abs_x(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->A>=oper;
    
    oper = cpu->A - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cmp abs y */
/*---------------------------------------*/
static void cpu_handle_cmp_abs_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_abs_y(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->A>=oper;
    
    oper = cpu->A - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cmp ind x */
/*---------------------------------------*/
static void cpu_handle_cmp_ind_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_ind_x(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->A>=oper;
    
    oper = cpu->A - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cmp ind y */
/*---------------------------------------*/
static void cpu_handle_cmp_ind_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_ind_y(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->A>=oper;
    
    oper = cpu->A - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* CPX OPERATION */

/*---------------------------------------------------*/
/* brief: handle cpx imm */
/*---------------------------------------*/
static void cpu_handle_cpx_imm(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper;
    oper = cpu_get_operand_byte(cpu, mem);

    cpu->carry = cpu->X>=oper;
    
    oper = cpu->X - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cpx zpg */
/*---------------------------------------*/
static void cpu_handle_cpx_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper = cpu_get_address_zpg(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->X>=oper;
    
    oper = cpu->X - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cpx abs */
/*---------------------------------------*/
static void cpu_handle_cpx_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_operand_short(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->X>=oper;
    
    oper = cpu->X - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* CPY OPERATION */

/*---------------------------------------------------*/
/* brief: handle cpy imm */
/*---------------------------------------*/
static void cpu_handle_cpy_imm(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper;
    oper = cpu_get_operand_byte(cpu, mem);

    cpu->carry = cpu->Y>=oper;
    
    oper = cpu->Y - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cpy zpg */
/*---------------------------------------*/
static void cpu_handle_cpy_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper = cpu_get_address_zpg(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->Y>=oper;
    
    oper = cpu->Y - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle cpy abs */
/*---------------------------------------*/
static void cpu_handle_cpy_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_operand_short(cpu, mem);
    oper = mem->data[oper];

    cpu->carry = cpu->Y>=oper;
    
    oper = cpu->Y - oper;

    cpu->zero = oper==0;
    cpu->neg = oper>>7;

    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* DEC OPERATION */

/*---------------------------------------------------*/
/* brief: handle dec zpg */
/*---------------------------------------*/
static void cpu_handle_dec_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t address = cpu_get_address_zpg(cpu, mem);
    mem->data[address]--;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle dec zpg x */
/*---------------------------------------*/
static void cpu_handle_dec_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint8_t address = cpu_get_address_zpg_x(cpu, mem);
    mem->data[address]--;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle dec abs */
/*---------------------------------------*/
static void cpu_handle_dec_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem);
    mem->data[address]--;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle dec abs x */
/*---------------------------------------*/
static void cpu_handle_dec_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_x(cpu, mem);
    mem->data[address]--;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* DEX OPERATION */

/*---------------------------------------------------*/
/* brief: handle dex imp */
/*---------------------------------------*/
static void cpu_handle_dex_imp(struct processor_t* cpu, struct mem* mem) {
    cpu->X--;

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->X_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* DEY OPERATION */

/*---------------------------------------------------*/
/* brief: handle dey imp */
/*---------------------------------------*/
static void cpu_handle_dey_imp(struct processor_t* cpu, struct mem* mem) {
    cpu->Y--;

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;

    cpu->Y_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* EOR OPERATION */

/*---------------------------------------------------*/
/* brief: handle eor imm */
/*---------------------------------------*/
static void cpu_handle_eor_imm(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper = cpu_get_operand_byte(cpu, mem);
    cpu->A = cpu->A ^ oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle eor zpg */
/*---------------------------------------*/
static void cpu_handle_eor_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper = cpu_get_address_zpg(cpu, mem);
    oper = mem->data[oper];

    cpu->A = cpu->A ^ oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle eor zpg x */
/*---------------------------------------*/
static void cpu_handle_eor_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper = cpu_get_address_zpg_x(cpu, mem);
    oper = mem->data[oper];
    
    cpu->A = cpu->A ^ oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle eor abs */
/*---------------------------------------*/
static void cpu_handle_eor_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_operand_short(cpu, mem);
    oper = mem->data[oper];
    
    cpu->A = cpu->A ^ oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle eor abs x */
/*---------------------------------------*/
static void cpu_handle_eor_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_abs_x(cpu, mem);
    oper = mem->data[oper];
    
    cpu->A = cpu->A ^ oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle eor abs y */
/*---------------------------------------*/
static void cpu_handle_eor_abs_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_abs_y(cpu, mem);
    oper = mem->data[oper];
    
    cpu->A = cpu->A ^ oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle eor ind x */
/*---------------------------------------*/
static void cpu_handle_eor_ind_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_ind_x(cpu, mem);
    oper = mem->data[oper];
    
    cpu->A = cpu->A ^ oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle eor ind y */
/*---------------------------------------*/
static void cpu_handle_eor_ind_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_ind_y(cpu, mem);
    oper = mem->data[oper];
    
    cpu->A = cpu->A ^ oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* INC OPERATION */

/*---------------------------------------------------*/
/* brief: handle inc zpg */
/*---------------------------------------*/
static void cpu_handle_inc_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t address = cpu_get_address_zpg(cpu, mem);
    mem->data[address]++;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle inc zpg x */
/*---------------------------------------*/
static void cpu_handle_inc_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint8_t address = cpu_get_address_zpg_x(cpu, mem);
    mem->data[address]++;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle inc abs */
/*---------------------------------------*/
static void cpu_handle_inc_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem);
    mem->data[address]++;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle inc abs x */
/*---------------------------------------*/
static void cpu_handle_inc_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_x(cpu, mem);
    mem->data[address]++;

    cpu->zero = mem->data[address]==0;
    cpu->neg = mem->data[address]>>7;

    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* INX OPERATION */

/*---------------------------------------------------*/
/* brief: handle inx imp */
/*---------------------------------------*/
static void cpu_handle_inx_imp(struct processor_t* cpu, struct mem* mem) {
    
    cpu->X++;

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->X_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* INY OPERATION */

/*---------------------------------------------------*/
/* brief: handle iny imp */
/*---------------------------------------*/
static void cpu_handle_iny_imp(struct processor_t* cpu, struct mem* mem) {
    
    cpu->Y++;

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;

    cpu->Y_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* JMP OPERATION */

/*---------------------------------------------------*/
/* brief: handle jmp abs */
/*---------------------------------------*/
static void cpu_handle_jmp_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem);
    cpu->PC = address;
    mem->last_selected = -1;
}

/*---------------------------------------------------*/
/* brief: handle jmp ind */
/*---------------------------------------*/
static void cpu_handle_jmp_ind(struct processor_t* cpu, struct mem* mem) {
    int8_t address = cpu_get_operand_byte(cpu, mem);
    cpu->PC += address;
}

/*---------------------------------------------------*/
/* JSR OPERATION */

/*---------------------------------------------------*/
/* brief: handle jsr abs */
/*---------------------------------------*/
static void cpu_handle_jsr_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem);

    mem->data[cpu->SP--] = cpu->PC & 0xff;
    mem->data[cpu->SP--] = cpu->PC>>8;

    cpu->PC = address;

    cpu->PC_st = true;
    cpu->SP_st = true;
    mem->last_selected = -1;
}

/*---------------------------------------------------*/
/* LDA OPERATION */

/*---------------------------------------------------*/
/* brief: handle lda imm */
/*---------------------------------------*/
static void cpu_handle_lda_imm(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper;
    oper = cpu_get_operand_byte(cpu, mem);
    cpu->A = oper;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->neg_st = true;
    cpu->zero_st = true;
    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle lda abs */
/*---------------------------------------*/
static void cpu_handle_lda_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address;
    address = cpu_get_operand_short(cpu, mem);
    cpu->A = mem->data[address];

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->neg_st = true;
    cpu->zero_st = true;
    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle lda zpg */
/*---------------------------------------*/
static void cpu_handle_lda_zpg(struct processor_t* cpu, struct mem* mem) {
    uint8_t address;
    address = cpu_get_address_zpg(cpu, mem);
    cpu->A = mem->data[address];
    
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;
    
    cpu->neg_st = true;
    cpu->zero_st = true;
    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle lda ind x */
/*---------------------------------------*/
static void cpu_handle_lda_ind_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_ind_x(cpu, mem);

    cpu->A = mem->data[oper];
    
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle lda ind y */
/*---------------------------------------*/
static void cpu_handle_lda_ind_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_ind_y(cpu, mem);

    cpu->A = mem->data[oper];
    
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle lda zpg x */
/*---------------------------------------*/
static void cpu_handle_lda_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint8_t oper = cpu_get_address_zpg_x(cpu, mem);

    cpu->A = mem->data[oper];
    
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle lda abs x */
/*---------------------------------------*/
static void cpu_handle_lda_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_abs_x(cpu, mem);

    cpu->A = mem->data[oper];
    
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle lda abs y */
/*---------------------------------------*/
static void cpu_handle_lda_abs_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t oper = cpu_get_address_abs_y(cpu, mem);

    cpu->A = mem->data[oper];
    
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* STA OPERATION */

/*---------------------------------------------------*/
/* brief: handle ldx imm*/
/*---------------------------------------*/
static void cpu_handle_ldx_imm(struct processor_t* cpu, struct mem* mem) {
    cpu->X = cpu_get_operand_byte(cpu, mem);

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->X_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* brief: handle ldx zpg*/
/*---------------------------------------*/
static void cpu_handle_ldx_zpg(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_zpg(cpu, mem);

    cpu->X = mem->data[address];

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->X_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* brief: handle ldx zpg y*/
/*---------------------------------------*/
static void cpu_handle_ldx_zpg_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_zpg_y(cpu, mem);

    cpu->X = mem->data[address];

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->X_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* brief: handle ldx abs*/
/*---------------------------------------*/
static void cpu_handle_ldx_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem);

    cpu->X = mem->data[address];

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->X_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* brief: handle ldx abs y*/
/*---------------------------------------*/
static void cpu_handle_ldx_abs_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_y(cpu, mem);

    cpu->X = mem->data[address];

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->X_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* STA OPERATION */

/*---------------------------------------------------*/
/* brief: handle ldy imm*/
/*---------------------------------------*/
static void cpu_handle_ldy_imm(struct processor_t* cpu, struct mem* mem) {
    cpu->Y = cpu_get_operand_byte(cpu, mem);

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;

    cpu->Y_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* brief: handle ldy zpg*/
/*---------------------------------------*/
static void cpu_handle_ldy_zpg(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_zpg(cpu, mem);

    cpu->Y = mem->data[address];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;

    cpu->Y_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* brief: handle ldy zpg x*/
/*---------------------------------------*/
static void cpu_handle_ldy_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_zpg_x(cpu, mem);

    cpu->Y = mem->data[address];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;

    cpu->Y_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* brief: handle ldy abs*/
/*---------------------------------------*/
static void cpu_handle_ldy_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem);

    cpu->Y = mem->data[address];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;

    cpu->Y_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* brief: handle ldy abs x*/
/*---------------------------------------*/
static void cpu_handle_ldy_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_x(cpu, mem);

    cpu->Y = mem->data[address];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;

    cpu->Y_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
    
}

/*---------------------------------------------------*/
/* LSR OPERATION */

/*---------------------------------------------------*/
/* brief: handle lsr acc*/
/*---------------------------------------*/
static void cpu_handle_lsr_acc(struct processor_t* cpu, struct mem* mem) {

    cpu->carry = cpu->A & 1;

    cpu->A = cpu->A>>1;
    
    cpu->zero = cpu->A==0;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;

}

/*---------------------------------------------------*/
/* brief: handle lsr zpg*/
/*---------------------------------------*/
static void cpu_handle_lsr_zpg(struct processor_t* cpu, struct mem* mem) {

    uint8_t address = cpu_get_address_zpg(cpu, mem);

    cpu->carry = mem->data[address] & 1;

    mem->data[address] = mem->data[address]>>1;
    
    cpu->zero = mem->data[address]==0;

    cpu->carry_st = true;
    cpu->zero_st = true;

}

/*---------------------------------------------------*/
/* brief: handle lsr zpg x*/
/*---------------------------------------*/
static void cpu_handle_lsr_zpg_x(struct processor_t* cpu, struct mem* mem) {

    uint16_t address = cpu_get_address_zpg_x(cpu, mem);

    cpu->carry = mem->data[address] & 1;

    mem->data[address] = mem->data[address]>>1;
    
    cpu->zero = mem->data[address]==0;

    cpu->carry_st = true;
    cpu->zero_st = true;

}

/*---------------------------------------------------*/
/* brief: handle lsr abs*/
/*---------------------------------------*/
static void cpu_handle_lsr_abs(struct processor_t* cpu, struct mem* mem) {

    uint8_t address = cpu_get_operand_short(cpu, mem);

    cpu->carry = mem->data[address] & 1;

    mem->data[address] = mem->data[address]>>1;
    
    cpu->zero = mem->data[address]==0;

    cpu->carry_st = true;
    cpu->zero_st = true;

}

/*---------------------------------------------------*/
/* brief: handle abs x*/
/*---------------------------------------*/
static void cpu_handle_lsr_abs_x(struct processor_t* cpu, struct mem* mem) {

    uint8_t address = cpu_get_address_abs_x(cpu, mem);

    cpu->carry = mem->data[address] & 1;

    mem->data[address] = mem->data[address]>>1;
    
    cpu->zero = mem->data[address]==0;

    cpu->carry_st = true;
    cpu->zero_st = true;

}

/*---------------------------------------------------*/
/* NOP OPERATION */

/*---------------------------------------------------*/
/* brief: handle nop */
/*---------------------------------------*/
static void cpu_handle_nop(struct processor_t* cpu, struct mem* mem) {

}

/*---------------------------------------------------*/
/* STA OPERATION */

/*---------------------------------------------------*/
/* brief: handle ora imm */
/*---------------------------------------*/
static void cpu_handle_ora_imm(struct processor_t* cpu, struct mem* mem) {

    cpu->A = cpu->A | cpu_get_operand_byte(cpu, mem);

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;
    
    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle ora zpg*/
/*---------------------------------------*/
static void cpu_handle_ora_zpg(struct processor_t* cpu, struct mem* mem) {

    uint16_t addr = cpu_get_address_zpg(cpu, mem);

    cpu->A = cpu->A | mem->data[addr];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;
    
    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle ora zpg x*/
/*---------------------------------------*/
static void cpu_handle_ora_zpg_x(struct processor_t* cpu, struct mem* mem) {

    uint16_t addr = cpu_get_address_zpg_x(cpu, mem);

    cpu->A = cpu->A | mem->data[addr];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;
    
    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle ora abs*/
/*---------------------------------------*/
static void cpu_handle_ora_abs(struct processor_t* cpu, struct mem* mem) {

    uint16_t addr = cpu_get_operand_short(cpu, mem);

    cpu->A = cpu->A | mem->data[addr];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;
    
    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle ora abs x*/
/*---------------------------------------*/
static void cpu_handle_ora_abs_x(struct processor_t* cpu, struct mem* mem) {

    uint16_t addr = cpu_get_address_abs_x(cpu, mem);

    cpu->A = cpu->A | mem->data[addr];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;
    
    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle ora abs y*/
/*---------------------------------------*/
static void cpu_handle_ora_abs_y(struct processor_t* cpu, struct mem* mem) {

    uint16_t addr = cpu_get_address_abs_y(cpu, mem);

    cpu->A = cpu->A | mem->data[addr];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;
    
    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle ora ind x*/
/*---------------------------------------*/
static void cpu_handle_ora_ind_x(struct processor_t* cpu, struct mem* mem) {

    uint16_t addr = cpu_get_address_ind_x(cpu, mem);

    cpu->A = cpu->A | mem->data[addr];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;
    
    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* brief: handle ora ind y*/
/*---------------------------------------*/
static void cpu_handle_ora_ind_y(struct processor_t* cpu, struct mem* mem) {

    uint16_t addr = cpu_get_address_ind_y(cpu, mem);

    cpu->A = cpu->A | mem->data[addr];

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;
    
    cpu->A_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;

}

/*---------------------------------------------------*/
/* PHA OPERATION */

/*---------------------------------------------------*/
/* brief: handle pha imp*/
/*---------------------------------------*/
static void cpu_handle_pha_imp(struct processor_t* cpu, struct mem* mem) {

    mem->data[cpu->SP--] = cpu->A;

    cpu->SP_st = true;

}

/*---------------------------------------------------*/
/* PHP OPERATION */

/*---------------------------------------------------*/
/* brief: handle php imp*/
/*---------------------------------------*/
static void cpu_handle_php_imp(struct processor_t* cpu, struct mem* mem) {

    uint8_t status = cpu->neg<<7 | cpu->over<<6 | cpu->dec<<3 | 
        cpu->ids<<2 | cpu->zero<<1 | cpu->carry;

    mem->data[cpu->SP--] = status;

    cpu->SP_st = true;

}

/*---------------------------------------------------*/
/* PLA OPERATION */

/*---------------------------------------------------*/
/* brief: handle pla imp*/
/*---------------------------------------*/
static void cpu_handle_pla_imp(struct processor_t* cpu, struct mem* mem) {

    cpu->A = mem->data[++cpu->SP];

    cpu->SP_st = true;
    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* RTS OPERATION */

/*---------------------------------------------------*/
/* brief: handle rts imp */
/*---------------------------------------*/
static void cpu_handle_rts_imp(struct processor_t* cpu, struct mem* mem) {
    
    cpu->PC = 0;
    cpu->PC = cpu->PC | mem->data[++cpu->SP]<<8;
    cpu->PC = (cpu->PC | mem->data[++cpu->SP]);

    cpu->SP_st = true;
    cpu->PC_st = true;
}

/*---------------------------------------------------*/
/* SBC OPERATION */

/*---------------------------------------------------*/
/* brief: handle sbc imm */
/*---------------------------------------*/
static void cpu_handle_sbc_imm(struct processor_t* cpu, struct mem* mem) {
    
    uint8_t oper = cpu_get_operand_byte(cpu, mem);

    uint8_t tmp = cpu->A;
    
    cpu->A = cpu->A - oper - cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sbc zpg */
/*---------------------------------------*/
static void cpu_handle_sbc_zpg(struct processor_t* cpu, struct mem* mem) {
    
    uint8_t oper = cpu_get_operand_byte(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A = cpu->A - mem->data[oper] - cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sbc zpg x */
/*---------------------------------------*/
static void cpu_handle_sbc_zpg_x(struct processor_t* cpu, struct mem* mem) {
    
    uint16_t oper = cpu_get_address_zpg_x(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A = cpu->A - mem->data[oper] - cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sbc abs */
/*---------------------------------------*/
static void cpu_handle_sbc_abs(struct processor_t* cpu, struct mem* mem) {
    
    uint16_t oper = cpu_get_operand_short(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A = cpu->A - mem->data[oper] - cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sbc abs x*/
/*---------------------------------------*/
static void cpu_handle_sbc_abs_x(struct processor_t* cpu, struct mem* mem) {
    
    uint16_t oper = cpu_get_address_abs_x(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A = cpu->A - mem->data[oper] - cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sbc abs y*/
/*---------------------------------------*/
static void cpu_handle_sbc_abs_y(struct processor_t* cpu, struct mem* mem) {
    
    uint16_t oper = cpu_get_address_abs_y(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A = cpu->A - mem->data[oper] - cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sbc ind x*/
/*---------------------------------------*/
static void cpu_handle_sbc_ind_x(struct processor_t* cpu, struct mem* mem) {
    
    uint16_t oper = cpu_get_address_ind_x(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A = cpu->A - mem->data[oper] - cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sbc ind y*/
/*---------------------------------------*/
static void cpu_handle_sbc_ind_y(struct processor_t* cpu, struct mem* mem) {
    
    uint16_t oper = cpu_get_address_ind_y(cpu, mem);

    uint8_t tmp = cpu->A;

    cpu->A = cpu->A - mem->data[oper] - cpu->carry;

    cpu->carry = cpu->A<tmp;
    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->A_st = true;
    cpu->carry_st = true;
    cpu->zero_st = true;
    cpu->neg_st = true;
}

/*---------------------------------------------------*/
/* SEC OPERATION */

/*---------------------------------------------------*/
/* brief: handle sec imp */
/*---------------------------------------*/
static void cpu_handle_sec_imp(struct processor_t* cpu, struct mem* mem) {
    
    cpu->carry = 1;

    cpu->carry_st = true;

}

/*---------------------------------------------------*/
/* SED OPERATION */

/*---------------------------------------------------*/
/* brief: handle sed imp */
/*---------------------------------------*/
static void cpu_handle_sed_imp(struct processor_t* cpu, struct mem* mem) {
    
    cpu->dec = 1;

    cpu->dec_st = true;

}

/*---------------------------------------------------*/
/* SEI OPERATION */

/*---------------------------------------------------*/
/* brief: handle sei imp */
/*---------------------------------------*/
static void cpu_handle_sei_imp(struct processor_t* cpu, struct mem* mem) {
    
    cpu->ids = 1;

    cpu->ids_st = true;

}

/*---------------------------------------------------*/
/* STA OPERATION */

/*---------------------------------------------------*/
/* brief: handle sta abs */
/*---------------------------------------*/
static void cpu_handle_sta_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address;
    address = cpu_get_operand_short(cpu, mem);
    mem->data[address] = cpu->A;

    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sta zpg */
/*---------------------------------------*/
static void cpu_handle_sta_zpg(struct processor_t* cpu, struct mem* mem) {
    uint16_t address;
    address = cpu_get_address_zpg(cpu, mem);
    mem->data[address] = cpu->A;

    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sta ind x*/
/*---------------------------------------*/
static void cpu_handle_sta_ind_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_ind_x(cpu, mem);
    mem->data[address] = cpu->A;

    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sta ind y*/
/*---------------------------------------*/
static void cpu_handle_sta_ind_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_ind_y(cpu, mem);
    mem->data[address] = cpu->A;

    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sta zpg x*/
/*---------------------------------------*/
static void cpu_handle_sta_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_zpg_x(cpu, mem);
    mem->data[address] = cpu->A;

    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sta abs x*/
/*---------------------------------------*/
static void cpu_handle_sta_abs_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_x(cpu, mem);
    mem->data[address] = cpu->A;

    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sta abs y*/
/*---------------------------------------*/
static void cpu_handle_sta_abs_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_abs_y(cpu, mem);
    mem->data[address] = cpu->A;

    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* STX OPERATION */

/*---------------------------------------------------*/
/* brief: handle stx zpg*/
/*---------------------------------------*/
static void cpu_handle_stx_zpg(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_byte(cpu, mem);
    mem->data[address] = cpu->X;

    cpu->X_st = true;
}

/*---------------------------------------------------*/
/* brief: handle stx zpg y*/
/*---------------------------------------*/
static void cpu_handle_stx_zpg_y(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_zpg_y(cpu, mem);
    mem->data[address] = cpu->X;

    cpu->X_st = true;
}

/*---------------------------------------------------*/
/* brief: handle stx abs*/
/*---------------------------------------*/
static void cpu_handle_stx_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem);
    mem->data[address] = cpu->X;

    cpu->X_st = true;
}

/*---------------------------------------------------*/
/* STY OPERATION */

/*---------------------------------------------------*/
/* brief: handle sty zpg*/
/*---------------------------------------*/
static void cpu_handle_sty_zpg(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_byte(cpu, mem);
    mem->data[address] = cpu->Y;

    cpu->Y_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sty zpg x*/
/*---------------------------------------*/
static void cpu_handle_sty_zpg_x(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_address_zpg_x(cpu, mem);
    mem->data[address] = cpu->Y;

    cpu->Y_st = true;
}

/*---------------------------------------------------*/
/* brief: handle sty abs*/
/*---------------------------------------*/
static void cpu_handle_sty_abs(struct processor_t* cpu, struct mem* mem) {
    uint16_t address = cpu_get_operand_short(cpu, mem);
    mem->data[address] = cpu->Y;

    cpu->Y_st = true;
}

/*---------------------------------------------------*/
/* TAX OPERATION */

/*---------------------------------------------------*/
/* brief: handle tax imp*/
/*---------------------------------------*/
static void cpu_handle_tax_imp(struct processor_t* cpu, struct mem* mem) {

    cpu->X = cpu->A;

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->neg_st = true;
    cpu->zero_st = true;

    cpu->X_st = true;
    cpu->A_st = true;
}

/*---------------------------------------------------*/
/* TAY OPERATION */

/*---------------------------------------------------*/
/* brief: handle tay imp*/
/*---------------------------------------*/
static void cpu_handle_tay_imp(struct processor_t* cpu, struct mem* mem) {

    cpu->Y = cpu->A;

    cpu->zero = cpu->Y==0;
    cpu->neg = cpu->Y>>7;

    cpu->neg_st = true;
    cpu->zero_st = true;

    cpu->A_st = true;
    cpu->Y_st = true;
}

/*---------------------------------------------------*/
/* TSX OPERATION */

/*---------------------------------------------------*/
/* brief: handle tsx imp*/
/*---------------------------------------*/
static void cpu_handle_tsx_imp(struct processor_t* cpu, struct mem* mem) {

    cpu->X = cpu->SP;

    cpu->zero = cpu->X==0;
    cpu->neg = cpu->X>>7;

    cpu->neg_st = true;
    cpu->zero_st = true;
    cpu->SP_st = true;
    cpu->X_st = true;
}

/*---------------------------------------------------*/
/* TXA OPERATION */

/*---------------------------------------------------*/
/* brief: handle txa imp*/
/*---------------------------------------*/
static void cpu_handle_txa_imp(struct processor_t* cpu, struct mem* mem) {

    cpu->A = cpu->X;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->neg_st = true;
    cpu->zero_st = true;

    cpu->A_st = true;
    cpu->X_st = true;
}

/*---------------------------------------------------*/
/* TXS OPERATION */

/*---------------------------------------------------*/
/* brief: handle txs imp*/
/*---------------------------------------*/
static void cpu_handle_txs_imp(struct processor_t* cpu, struct mem* mem) {

    cpu->SP = cpu->X;

    cpu->SP_st = true;
    cpu->X_st = true;
}

/*---------------------------------------------------*/
/* TYA OPERATION */

/*---------------------------------------------------*/
/* brief: handle tya imp*/
/*---------------------------------------*/
static void cpu_handle_tya_imp(struct processor_t* cpu, struct mem* mem) {

    cpu->A = cpu->Y;

    cpu->zero = cpu->A==0;
    cpu->neg = cpu->A>>7;

    cpu->neg_st = true;
    cpu->zero_st = true;

    cpu->Y_st = true;
    cpu->A_st = true;
}

struct cpu_op_handler_t op_handler[] = {
    /* ADC */
    {ADC_IMM,   cpu_handle_adc_imm},
    {ADC_ABS,   cpu_handle_adc_abs},
    {ADC_ZPG,   cpu_handle_adc_zpg},
    {ADC_IND_X, cpu_handle_adc_ind_x}, 
    {ADC_IND_Y, cpu_handle_adc_ind_y},
    {ADC_ZPG_X, cpu_handle_adc_zpg_x},
    {ADC_ABS_X, cpu_handle_adc_abs_x},
    {ADC_ABS_Y, cpu_handle_adc_abs_y},

    /* AND */
    {AND_IMM,   cpu_handle_and_imm},
    {AND_ABS,   cpu_handle_and_abs},
    {AND_ZPG,   cpu_handle_and_zpg},
    {AND_IND_X, cpu_handle_and_ind_x},
    {AND_IND_Y, cpu_handle_and_ind_y}, 
    {AND_ZPG_X, cpu_handle_and_zpg_x},
    {AND_ABS_X, cpu_handle_and_abs_x},
    {AND_ABS_Y, cpu_handle_and_abs_y},

    /* ASL */
    {ASL_ACC,   cpu_handle_asl_acc},
    {ASL_ZPG,   cpu_handle_asl_zpg}, 
    {ASL_ZPG_X, cpu_handle_asl_zpg_x},
    {ASL_ABS,   cpu_handle_asl_abs}, 
    {ASL_ABS_X, cpu_handle_asl_abs_x},

    /* BCC */
    {BCC_REL,   cpu_handle_bcc_rel},
    
    /* BCS */
    {BCS_REL,   cpu_handle_bcs_rel},
   
    /* BEQ */
    {BEQ_REL,   cpu_handle_beq_rel},
    
    /* BIT */
    {BIT_ZPG,   NULL}, //TO ADD
    {BIT_ABS,   NULL}, //TO ADD
    
    /* BMI */
    {BMI_REL,   cpu_handle_bmi_rel},
    
    /* BNE */
    {BNE_REL,   cpu_handle_bne_rel},
    
    /* BPL */   
    {BPL_REL,   cpu_handle_bpl_rel},
    
    /* BRK */   
    {BRK_IMP,   NULL}, //TO ADD
    
    /* BVC */ 
    {BVC_REL,   cpu_handle_bvc_rel},

    /* BVS */ 
    {BVS_REL,   cpu_handle_bvs_rel},
    
    /* CLC */ 
    {CLC_IMP,   cpu_handle_clc_imp},
    
    /* CLD */ 
    {CLD_IMP,   cpu_handle_cld_imp},
    
    /* CLI */ 
    {CLI_IMP,   cpu_handle_cli_imp},
    
    /* CLV */ 
    {CLV_IMP,   cpu_handle_clv_imp},
    
    /* CMP */ 
    {CMP_IMM,   cpu_handle_cmp_imm},
    {CMP_ZPG,   cpu_handle_cmp_zpg},
    {CMP_ZPG_X, cpu_handle_cmp_zpg_x},
    {CMP_ABS,   cpu_handle_cmp_abs},
    {CMP_ABS_X, cpu_handle_cmp_abs_x},
    {CMP_ABS_Y, cpu_handle_cmp_abs_y},
    {CMP_IND_X, cpu_handle_cmp_ind_x},
    {CMP_IND_Y, cpu_handle_cmp_ind_y},
    
    /* CPX */
    {CPX_IMM,   cpu_handle_cpx_imm},
    {CPX_ZPG,   cpu_handle_cpx_zpg},
    {CPX_ABS,   cpu_handle_cpx_abs},
    
    /* CPY */
    {CPY_IMM,   cpu_handle_cpy_imm}, 
    {CPY_ZPG,   cpu_handle_cpy_zpg}, 
    {CPY_ABS,   cpu_handle_cpy_abs}, 
    
    /* DEC */
    {DEC_ZPG,   cpu_handle_dec_zpg},
    {DEC_ZPG_X, cpu_handle_dec_zpg_x},
    {DEC_ABS,   cpu_handle_dec_abs},
    {DEC_ABS_X, cpu_handle_dec_abs_x},
    
    /* DEX */
    {DEX_IMP,   cpu_handle_dex_imp},
    
    /* DEY */
    {DEY_IMP,   cpu_handle_dey_imp},
   
    /* EOR */
    {EOR_IMM,   cpu_handle_eor_imm},
    {EOR_ZPG,   cpu_handle_eor_zpg}, 
    {EOR_ZPG_X, cpu_handle_eor_zpg_x}, 
    {EOR_ABS,   cpu_handle_eor_abs},
    {EOR_ABS_X, cpu_handle_eor_abs_x},
    {EOR_ABS_Y, cpu_handle_eor_abs_y},
    {EOR_IND_X, cpu_handle_eor_ind_x},
    {EOR_IND_Y, cpu_handle_eor_ind_y},
    
    /* INC */
    {INC_ZPG,   cpu_handle_inc_zpg},
    {INC_ZPG_X, cpu_handle_inc_zpg_x},
    {INC_ABS,   cpu_handle_inc_abs},
    {INC_ABS_X, cpu_handle_inc_abs_x},
    
    /* INX */
    {INX_IMP,   cpu_handle_inx_imp},
    
    /* INY */
    {INY_IMP,   cpu_handle_iny_imp},
    
    /* JMP */
    {JMP_ABS,   cpu_handle_jmp_abs},
    {JMP_IND,   cpu_handle_jmp_ind},
    
    /* JSR */
    {JSR_ABS,   cpu_handle_jsr_abs}, 

    /* LDA */
    {LDA_IMM,   cpu_handle_lda_imm},
    {LDA_ABS,   cpu_handle_lda_abs},
    {LDA_ZPG,   cpu_handle_lda_zpg},
    {LDA_IND_X, cpu_handle_lda_ind_x},
    {LDA_IND_Y, cpu_handle_lda_ind_y},
    {LDA_ZPG_X, cpu_handle_lda_zpg_x},
    {LDA_ABS_X, cpu_handle_lda_abs_x},
    {LDA_ABS_Y, cpu_handle_lda_abs_y},


    /* LDX */
    {LDX_IMM,   cpu_handle_ldx_imm},
    {LDX_ZPG,   cpu_handle_ldx_zpg},
    {LDX_ZPG_Y, cpu_handle_ldx_zpg_y},
    {LDX_ABS,   cpu_handle_ldx_abs}, 
    {LDX_ABS_Y, cpu_handle_ldx_abs_y},

    /* LDY */
    {LDY_IMM,   cpu_handle_ldy_imm},
    {LDY_ZPG,   cpu_handle_ldy_zpg},
    {LDY_ZPG_X, cpu_handle_ldy_zpg_x},
    {LDY_ABS,   cpu_handle_ldy_abs}, 
    {LDY_ABS_X, cpu_handle_ldy_abs_x},

    /* LSR */
    {LSR_ACC,   cpu_handle_lsr_acc},
    {LSR_ZPG,   cpu_handle_lsr_zpg},
    {LSR_ZPG_X, cpu_handle_lsr_zpg_x},
    {LSR_ABS,   cpu_handle_lsr_abs}, 
    {LSR_ABS_X, cpu_handle_lsr_abs_x},
    
    /* NOP */
    {NOP,  cpu_handle_nop}, 
    
    /* ORA */
    {ORA_IMM,   cpu_handle_ora_imm}, 
    {ORA_ZPG,   cpu_handle_ora_zpg},
    {ORA_ZPG_X, cpu_handle_ora_zpg_x},
    {ORA_ABS,   cpu_handle_ora_abs}, 
    {ORA_ABS_X, cpu_handle_ora_abs_x},
    {ORA_ABS_Y, cpu_handle_ora_abs_y},
    {ORA_IND_X, cpu_handle_ora_ind_x},
    {ORA_IND_Y, cpu_handle_ora_ind_y},
    
    /* PHA */
    {PHA_IMP,   cpu_handle_pha_imp}, 
    
    /* PHP */
    {PHP_IMP,   cpu_handle_php_imp}, 
    
    /* PLA */
    {PLA_IMP,   cpu_handle_pla_imp}, 
    
    /* PLP */
    {PLP_IMP,   NULL}, //TO ADD

    /* ROL */
    {ROL_ACC,   NULL}, //TO ADD
    {ROL_ZPG,   NULL}, //TO ADD
    {ROL_ZPG_X,   NULL}, //TO ADD
    {ROL_ABS,   NULL}, //TO ADD
    {ROL_ABS_X,   NULL}, //TO ADD
    
    /* ROR */
    {ROR_ACC,   NULL}, //TO ADD
    {ROR_ZPG,   NULL}, //TO ADD
    {ROR_ZPG_X,   NULL}, //TO ADD
    {ROR_ABS,   NULL}, //TO ADD
    {ROR_ABS_X,   NULL}, //TO ADD
    
    /* RTI */
    {RTI_IMP,   NULL}, //TO ADD
    
    /* RTS */
    {RTS_IMP,   cpu_handle_rts_imp},
    
    /* SBC */
    {SBC_IMM,   cpu_handle_sbc_imm},
    {SBC_ZPG,   cpu_handle_sbc_zpg}, 
    {SBC_ZPG_X, cpu_handle_sbc_zpg_x},
    {SBC_ABS,   cpu_handle_sbc_abs}, 
    {SBC_ABS_X, cpu_handle_sbc_abs_x},
    {SBC_ABS_Y, cpu_handle_sbc_abs_y},
    {SBC_IND_X, cpu_handle_sbc_ind_x},
    {SBC_IND_Y, cpu_handle_sbc_ind_y}, 
    
    /* SEC */
    {SEC_IMP,   cpu_handle_sec_imp},
    
    /* SED */
    {SED_IMP,   cpu_handle_sed_imp},
    
    /* SEI */
    {SEI_IMP,   cpu_handle_sei_imp}, 
   
    /* STA */
    {STA_ABS,   cpu_handle_sta_abs},
    {STA_ZPG,   cpu_handle_sta_zpg},
    {STA_IND_X, cpu_handle_sta_ind_x}, 
    {STA_IND_Y, cpu_handle_sta_ind_y},
    {STA_ZPG_X, cpu_handle_sta_zpg_x},
    {STA_ABS_X, cpu_handle_sta_abs_x},
    {STA_ABS_Y, cpu_handle_sta_abs_y},

    /* STX */
    {STX_ZPG,   cpu_handle_stx_zpg},
    {STX_ZPG_Y, cpu_handle_stx_zpg_y},
    {STX_ABS,   cpu_handle_stx_abs}, 
    
    /* STY */
    {STY_ZPG,   cpu_handle_sty_zpg},
    {STY_ZPG_X, cpu_handle_sty_zpg_x}, 
    {STY_ABS,   cpu_handle_sty_abs}, 
    
    /* TAX */
    {TAX_IMP,   cpu_handle_tax_imp},
    
    /* TAY */
    {TAY_IMP,   cpu_handle_tay_imp},
    
    /* TSX */
    {TSX_IMP,   cpu_handle_tsx_imp},
    
    /* TXA */
    {TXA_IMP,   cpu_handle_txa_imp},
    
    /* TXS */
    {TXS_IMP,   cpu_handle_txs_imp},
    
    /* TYA */
    {TYA_IMP,   cpu_handle_tya_imp}, 
};

/*---------------------------------------------------*/
/* brief: return the next operand short */
/*---------------------------------------*/
int cpu_step(struct processor_t* cpu, struct mem* mem, enum opcode_e op) {

    mem->last_selected = -1;
    cpu->A_st = false;
    cpu->X_st = false;
    cpu->Y_st = false;
    cpu->PC_st = false;
    cpu->SP_st = false;

    for(int i=0; i<sizeof(op_handler)/sizeof(struct cpu_op_handler_t); i++) {
        if(op==op_handler[i].op) {
            if(op_handler[i].operation==NULL) {
                cpu_handle_unsupported(op);
            } else {
                op_handler[i].operation(cpu, mem);
            }
            return 0;
        }
    }

    return 1;
  
}
