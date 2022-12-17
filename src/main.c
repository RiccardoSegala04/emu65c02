#include <stdio.h>
#include <memory.h>
#include <processor.h>
#include <common.h>
#include <emulator.h>
#include <log.h>


int main(int argc, char* argv[]) {

    LOG_INIT("debug.log")

    if(argc==2) {

        
        struct processor_t cpu;
        cpu_init(&cpu);

        struct mem mem;
        mem_init(&mem);
        int rc = mem_load(&mem, argv[1]);

        cpu_load_res_addr(&cpu, &mem);

        if(rc!=0) {
            cpu.is_running = false;
        }
    
        struct emulator_t emu;
        emu_init(&emu, &mem);

        emu_display_commands(&emu.commands, emu.show_io);


        enum opcode_e op;
    
        while(cpu.is_running) {

            emu_display(&emu, &cpu, &mem);
            int ch = getch();

            switch(ch) {
                case 'q':
                    cpu.is_running = false;
                    break;
                case 'r':
                    cpu_init(&cpu);
                    cpu_load_res_addr(&cpu, &mem);
                    break;
                case 's':
                    op = cpu_fetch(&cpu, &mem);
                    if(cpu_step(&cpu, &mem, op)!=0) {
                        cpu.is_running = false;
                    }
                    mem_set_btn(&mem, cpu.button_pressed);
                    break;
                case 'b':
                    cpu.button_pressed = !cpu.button_pressed;
                    mem_set_btn(&mem, cpu.button_pressed);
                    break;
                case KEY_RESIZE:
                    emu_refresh(&emu, &cpu, &mem);
                    break;
            }   
        }

        emu_dispose(&emu);
    }

    LOG_CLOSE();

    return 0;
}
