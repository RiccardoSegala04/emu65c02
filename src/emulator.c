#include <emulator.h>
#include <common.h>
#include <string.h>
#include <log.h>

const char EMU_LED_CHAR[] = {'0', '*'};

static void dump_oper(
    struct processor_t *cpu, struct mem* mem, uint16_t addr, 
    int bytes, int line, WINDOW* win) 
{
    
    if(cpu->PC==addr)
        wattron(win, COLOR_PAIR(EMU_SHOW_COLOR));

    wmove(win, line, 0);
    for(int i=0; i<EMU_PROGRAM_COLS-10; i++) {
        waddch(win, ' ');
    }

    mvwprintw(win, line, 0, "%04x %02x ", addr, mem->data[addr]);
    
    for(int i=1; i<=bytes; i++) {
        wprintw(win, "%02x ", mem->data[addr+i]);
    }

    const char* name = cpu_get_op_name(mem->data[addr]);

    if(name==NULL)
        name = "?";

    mvwprintw(win, line, 17, "%s ", name);
    
    enum processor_op_type_e op_t = cpu_get_op_type(mem->data[addr]);

    if(op_t == OP_IMM) {
        wprintw(win, "#");
    } else if(op_t == OP_X_IND){ 
        wprintw(win, "X,");
    }
    
    
    if(bytes==1) {
        wprintw(win, "$%02x", mem->data[addr+1]);
    } else if(bytes==2) {
        wprintw(win, "$%02x%02x ", mem->data[addr+2], mem->data[addr+1]);
    }

    if(op_t == OP_ABS_X || op_t == OP_ZPG_X) {
        wprintw(win, ",X");
    } else if(op_t == OP_ABS_Y || op_t == OP_ZPG_Y || op_t == OP_IND_Y) {
        wprintw(win, ",Y");
    }

    if(cpu->PC==addr)
        wattroff(win, COLOR_PAIR(EMU_SHOW_COLOR));

    wrefresh(win);
}

static uint16_t get_addr_next_op(int ops, uint16_t addr, struct mem* mem) {

    int bytes = 0;
    for(int i=0; i<ops; i++) {
        int op_bytes = cpu_op_get_n_bytes(mem->data[addr+i+bytes]);

        bytes += op_bytes+1;
    }

    return addr+bytes;
}

int emu_init(struct emulator_t* emu, struct mem* mem) {

    bzero(emu, sizeof(*emu));

    emu->inst_p = mem->data[MEM_RES] | mem->data[MEM_RES+1]<<8;

    initscr();

    start_color();
    init_pair(EMU_SHOW_COLOR, COLOR_BLACK, COLOR_CYAN);
    init_pair(EMU_DISPLAY_COLOR, COLOR_BLACK, COLOR_GREEN);
    init_pair(EMU_LED_COLOR, COLOR_WHITE, COLOR_RED);
    use_default_colors();

    curs_set(0);
    noecho();

    refresh();

    int rc = 0;

    rc = emu_section_init(
            &emu->program, 0, 0, EMU_PROGRAM_LINES, EMU_PROGRAM_COLS, "Code");
    RET_ON_ERR(rc);

    rc = emu_section_init(
            &emu->data, 4, 40, EMU_DATA_LINES, EMU_DATA_COLS, "Data");
    RET_ON_ERR(rc);

    rc = emu_section_init(&emu->commands, 18, 0, 6, 40, "Commands");
    RET_ON_ERR(rc);

    rc = emu_section_init(&emu->registers, 16, 40, 8, 40, "Registers");
    RET_ON_ERR(rc);
    
    rc = emu_section_init(
            &emu->io, 0, EMU_PROGRAM_COLS, 4, EMU_DATA_COLS, "I/O");
    RET_ON_ERR(rc);

    return 0;
}

int emu_dispose(struct emulator_t* emu) {

    int rc;

    rc = emu_section_dispose(&emu->program);
    RET_ON_ERR(rc);

    rc = emu_section_dispose(&emu->data);
    RET_ON_ERR(rc);

    rc = emu_section_dispose(&emu->commands);
    RET_ON_ERR(rc);

    rc = emu_section_dispose(&emu->registers);
    RET_ON_ERR(rc);

    return endwin();
}

void emu_display(
    struct emulator_t* emu, 
    struct processor_t *cpu, 
    struct mem* mem) 
{
    emu_dump_registers(cpu, &emu->registers);
    emu_dump_program(cpu, mem, emu);
    emu_dump_data(mem, emu);
    emu_display_io(cpu, mem, &emu->io);
}

int emu_section_init(
    struct emu_section_t* section, int y, int x, 
    int h, int w, const char* name) 
{

    section->border = newwin(h, w, y, x);
    box(section->border, 0, 0);
    mvwprintw(section->border, 0, 2, "[%s]", name);

    wrefresh(section->border);
    

    section->inner = newwin(h-2, w-2, y+1, x+1);

    refresh();

    return 0;
}

int emu_section_dispose(struct emu_section_t* section) {

    delwin(section->border);
    delwin(section->inner);

    return 0;
}

void emu_display_commands(
        struct emu_section_t* commands, bool show_io) {

    wclear(commands->inner);

    mvwprintw(commands->inner, 0, 1, "s - step");
    mvwprintw(commands->inner, 1, 1, "r - reset");
    mvwprintw(commands->inner, 2, 1, "b - toggle button");
    mvwprintw(commands->inner, 3, 1, "q - quit");
    wrefresh(commands->inner);
}

void emu_dump_program(
    struct processor_t *cpu, struct mem* mem, struct emulator_t* emu) 
{

    int diff = cpu->PC-emu->inst_p;
//  LOG_DEBUG("PC-INST_P = %d. PC=0x%04x - INST_P=0x%04x",
//          diff, cpu->PC, emu->inst_p);

    if(diff<0 || diff>EMU_PROGRAM_LINES)
        emu->inst_p = cpu->PC;
    else if(diff>EMU_PROGRAM_LINES-3)
        emu->inst_p = get_addr_next_op(1, emu->inst_p, mem);

    int bytes = 0;
    for(int i=0; i<EMU_PROGRAM_LINES-2; i++) {
        int op_bytes = cpu_op_get_n_bytes(mem->data[emu->inst_p+i+bytes]);
        
        dump_oper(
            cpu, mem, emu->inst_p+bytes+i, op_bytes, i, emu->program.inner);

        bytes += op_bytes;
    }
}

void emu_dump_data(struct mem* mem, struct emulator_t* emu) {
    
    if(mem->last_selected!=-1) {
        int diff = mem->last_selected-emu->mem_p;
        if(diff>EMU_DATA_ROW_CELLS*(EMU_DATA_LINES-2) || diff<0) {
            emu->mem_p = mem->last_selected;
        }
    }

    for(int i=0; i<EMU_DATA_LINES-2; i++) {
        wmove(emu->data.inner, i, 0);
        wprintw(emu->data.inner, "%04x ", emu->mem_p+i*EMU_DATA_ROW_CELLS);
        for(int j=0; j<EMU_DATA_ROW_CELLS; j++) {
            uint16_t cur_addr = emu->mem_p+i*EMU_DATA_ROW_CELLS+j;
            
            if(cur_addr == mem->last_selected) 
                wattron(emu->data.inner, COLOR_PAIR(EMU_SHOW_COLOR));

            wprintw(emu->data.inner, "%02x ", mem->data[emu->mem_p+i*EMU_DATA_ROW_CELLS+j]);
            
            if(cur_addr == mem->last_selected) 
                wattroff(emu->data.inner, COLOR_PAIR(EMU_SHOW_COLOR));
        }
        for(int j=0; j<EMU_DATA_ROW_CELLS; j++) {
            uint16_t cur_addr = emu->mem_p+i*EMU_DATA_ROW_CELLS+j;
            
            if(cur_addr == mem->last_selected) 
                wattron(emu->data.inner, COLOR_PAIR(EMU_SHOW_COLOR));

            waddch(emu->data.inner, '.');

            if(cur_addr == mem->last_selected) 
                wattroff(emu->data.inner, COLOR_PAIR(EMU_SHOW_COLOR));
        }
    }

    wrefresh(emu->data.inner);
}

void emu_dump_registers(
        struct processor_t *cpu, struct emu_section_t* registers) 
{
    mvwprintw(registers->inner, 0, 1, "Accumulator........ : ");

    if(cpu->A_st)
        wattron(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    wprintw(registers->inner, "%02x (%03d)", 
            cpu->A, cpu->A
    );

    if(cpu->A_st)
        wattroff(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    mvwprintw(registers->inner, 1, 1, "X.................. : ");

    if(cpu->X_st)
        wattron(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    wprintw(registers->inner, "%02x (%03d)", 
        cpu->X, cpu->X
    );

    if(cpu->X_st)
        wattroff(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    mvwprintw(registers->inner, 2, 1, "Y.................. : ");

    if(cpu->Y_st)
        wattron(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    wprintw(registers->inner, "%02x (%03d)", 
        cpu->Y, cpu->Y
    );

    if(cpu->Y_st)
        wattroff(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    mvwprintw(registers->inner, 3, 1, "Stack Pointer...... : ");

    if(cpu->SP_st)
        wattron(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    wprintw(registers->inner, "%02x", cpu->SP);

    if(cpu->SP_st)
        wattroff(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    mvwprintw(registers->inner, 4, 1, "Program Counter.... : ");

    if(cpu->PC_st)
        wattron(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    wprintw(registers->inner, "%04x", cpu->PC);

    wattroff(registers->inner, COLOR_PAIR(EMU_SHOW_COLOR));

    mvwprintw(registers->inner, 5, 1, 
        "Flags.............. : %d%d%d%d%d%d%d%d\n", 
        cpu->neg, cpu->over, 0, cpu->brk, cpu->dec, 
        cpu->ids, cpu->zero, cpu->carry
    );
    
    wrefresh(registers->inner);
}

char emu_led_char(struct mem* mem, uint8_t mask) {
    return EMU_LED_CHAR[
        (mem->data[MEM_DDRB] & mask) && (mem->data[MEM_PTB] & mask)
    ];
}

void emu_display_io(
        struct processor_t *cpu, struct mem *mem, struct emu_section_t* io) {
    
    wattron(io->inner, COLOR_PAIR(EMU_DISPLAY_COLOR));

    mvwprintw(io->inner, 0, 1, "                ");
    mvwprintw(io->inner, 1, 1, "                ");

    wattroff(io->inner, COLOR_PAIR(EMU_DISPLAY_COLOR));

    wattron(io->inner, COLOR_PAIR(EMU_LED_COLOR));
    mvwprintw(io->inner, 0, 25, "%c", emu_led_char(mem, 1<<3));
    mvwprintw(io->inner, 0, 26, "%c", emu_led_char(mem, 1<<2));
    mvwprintw(io->inner, 0, 27, "%c", emu_led_char(mem, 1<<1));
    mvwprintw(io->inner, 0, 28, "%c", emu_led_char(mem, 1));
    wattroff(io->inner, COLOR_PAIR(EMU_LED_COLOR));
    mvwprintw(io->inner, 1, 22, "Button: [%c]", 
            cpu->button_pressed ? 'x' : ' ');

    wrefresh(io->inner);
}

void emu_refresh_section(struct emu_section_t* sec, const char* name) {
    wclear(sec->border);
    box(sec->border, 0, 0);
    mvwprintw(sec->border, 0, 2, "[%s]", name);
    wclear(sec->inner);
    wrefresh(sec->border);
    wrefresh(sec->inner);
}

void emu_refresh(
    struct emulator_t* emu, struct processor_t *cpu, struct mem* mem) {
    
    emu_refresh_section(&emu->program, "Code");
    emu_refresh_section(&emu->data, "Data");
    emu_refresh_section(&emu->commands, "Commands");
    emu_refresh_section(&emu->registers, "Registers");
    emu_refresh_section(&emu->io, "I/O");
    
    emu_display(emu, cpu, mem);
    emu_display_commands(&emu->commands, emu->show_io);
}

