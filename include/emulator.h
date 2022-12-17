#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include <mem.h>
#include <processor.h>
#include <ncurses.h>

#define EMU_SHOW_COLOR 1
#define EMU_DISPLAY_COLOR 2
#define EMU_LED_COLOR 3

#define EMU_PROGRAM_LINES 18
#define EMU_PROGRAM_COLS 40

#define EMU_DATA_LINES 12
#define EMU_DATA_COLS 40

#define EMU_DATA_ROW_CELLS 8

extern const char EMU_LED_CHAR[];

struct emu_section_t {
    WINDOW* border;
    WINDOW* inner;
};

struct emulator_t {
    struct emu_section_t program;
    struct emu_section_t data;
    struct emu_section_t commands;
    struct emu_section_t registers;
    struct emu_section_t io;

    uint16_t inst_p;
    uint16_t mem_p;
    bool show_io;
};


int emu_init(struct emulator_t* emu, struct mem* mem);
int emu_dispose(struct emulator_t* emu);
void emu_display(
        struct emulator_t* emu, 
        struct processor_t *cpu, 
        struct mem* mem
);

int emu_section_init(
        struct emu_section_t* section, 
        int y, 
        int x, 
        int h, 
        int w, 
        const char* name
);

int emu_section_dispose(struct emu_section_t* section);

void emu_display_commands(struct emu_section_t* commands, bool show_io);

void emu_dump_program(
        struct processor_t *cpu, 
        struct mem* mem, 
        struct emulator_t* emu
);
void emu_dump_data(struct mem* mem, struct emulator_t* emu);
void emu_dump_registers(
        struct processor_t *cpu, 
        struct emu_section_t* registers
);
void emu_display_io(
        struct processor_t *cpu, struct mem *mem, struct emu_section_t* io);
void emu_refresh(
        struct emulator_t* emu, struct processor_t *cpu, struct mem* mem);

#endif
