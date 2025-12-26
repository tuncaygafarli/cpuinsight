#pragma once
#include "gui_renderer.h"
#include "../cpu/cpu.h"
namespace gui {

    class cpu_reset_button_t {

    public:
        cpu_reset_button_t(CPU* cpu) : _cpu(cpu) {}
        void draw();
    private:
        CPU* _cpu = nullptr;
    };
} // namespace gui