#pragma once
#include <string>
#include <vector>
#include "gui_renderer.h"
namespace gui {

    class assembly_element_t {

    public:
        assembly_element_t(std::vector<std::string>&& instructions,const size_t* pc) : _assembly(std::move(instructions)),_cpu_pc(pc) {}
        void draw();
    private:
        std::vector<std::string> _assembly;
        const size_t* _cpu_pc = nullptr;
    };
} // namespace gui