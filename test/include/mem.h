#ifndef __MEM_H__
#define __MEM_H__ 

#include <common.h>
#include <stdbool.h>

#define MEM_CODE_ADDR   0x8000
#define MEM_SIZE        0xffff+1

#define MEM_IRQ 0xfffe
#define MEM_RES 0xfffc
#define MEM_NMI 0xfffa

// I/O addresses
#define MEM_PTA  0x4000
#define MEM_PTB  0x4001
#define MEM_DDRA 0x4002
#define MEM_DDRB 0x4003

struct mem {
    uint8_t data[MEM_SIZE];
    int last_selected;
};

void mem_init(struct mem* m);
int mem_load(struct mem* m, char* filename);
uint16_t mem_get_data_short(struct mem* m, uint16_t src);
uint8_t mem_get_data_byte(struct mem* m, uint16_t src);

int mem_set_btn(struct mem* m, bool pressed);

#endif
