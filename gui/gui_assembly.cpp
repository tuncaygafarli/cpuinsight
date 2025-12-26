#include "gui_assembly.h"
#include <cmath>
namespace gui {
	void assembly_element_t::draw() {

        ImGui::SetNextWindowPos(ImVec2(0, 0));      // top-left corner
        ImGui::SetNextWindowSize(ImVec2(300, renderer::get_window_size().y));   // fixed size
        // Full-screen child
        ImGui::Begin("Assembly",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse
        );


        ImVec2 size = ImGui::GetContentRegionAvail(); 
        ImGui::BeginChild("ASMView", size, true,
            ImGuiWindowFlags_HorizontalScrollbar);


        float line_height = ImGui::GetTextLineHeightWithSpacing();
        uint32_t total_to_show = 40;

        uint32_t first_index = std::max(0u, static_cast<uint32_t>(*_cpu_pc));
        uint32_t last_index  = std::min((uint32_t)_assembly.size(), first_index + total_to_show);

        ImGui::SetScrollY(first_index * line_height);
        // Clipper for assembly
        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int32_t>(_assembly.size()));
        while (clipper.Step()) {
            for (int32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                const bool is_pc = (i == *_cpu_pc);
                if (is_pc) {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
                    ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(255, 100, 100, 60));
                    ImGui::Selectable(_assembly[i].c_str(), true,
                        ImGuiSelectableFlags_SpanAllColumns);
                    ImGui::PopStyleColor(2);
                } else {
                    ImGui::TextUnformatted((std::to_string(i+1) + ". " + _assembly[i]).c_str());
                }
            }
        }

        ImGui::EndChild();
        ImGui::End();

	}
}

