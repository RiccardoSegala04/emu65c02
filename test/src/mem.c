#include <mem.h>
#include <stdio.h>
#include <string.h>

/*---------------------------------------------------*/
/* brief: init the memory struct */
/*---------------------------------------*/
void mem_init(struct mem* m) {
    memset(m, 0, sizeof(*m));
    m->last_selected = -1;
}

/*---------------------------------------------------*/
/* brief: load a program from the disk */
/*---------------------------------------*/
int mem_load(struct mem* m, char* filename) {

    FILE* fp = NULL;
    fp = fopen(filename, "r");

    if(fp==NULL) {
        return 1;
    }

    int index = 0;
    int read = 0;
    while((read = fread(m->data+index+MEM_CODE_ADDR, 1, 128, fp))) {
        index+=read;
    }

    return 0;
}

/*---------------------------------------------------*/
/* brief: return the 16 bit data after src */
/*---------------------------------------*/
uint16_t mem_get_data_short(struct mem* m, uint16_t src) {
    uint16_t param = m->data[src];

    param = param | (m->data[src+1]<<8);

    return param;
}

/*---------------------------------------------------*/
/* brief: return the 8 bit data after src */
/*---------------------------------------*/
uint8_t mem_get_data_byte(struct mem* m, uint16_t src) {
    return m->data[src];
}

/*---------------------------------------------------*/
/* brief: set the status of the hardware btn */
/*---------------------------------------*/
int mem_set_btn(struct mem* m, bool pressed) {
    if(pressed && ((m->data[MEM_DDRB] & 1<<4)==0))
        m->data[MEM_PTB] = m->data[MEM_PTB] | 1<<4;
    else
        m->data[MEM_PTB] = m->data[MEM_PTB] & (~(1<<4));

    return 0;
}
