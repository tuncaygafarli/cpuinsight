#include <string>
#include <iomanip>
#include <sstream>

#include "gui_reg_file.h"
#include "gui_renderer.h"
#include "../parser/lookup.h"

namespace gui {

    void reg_file_element_t::draw() {


        auto window_size = renderer::get_window_size();

        ImGui::SetNextWindowPos(ImVec2(window_size.x-300, 0));      // top-left corner
        ImGui::SetNextWindowSize(ImVec2(300, window_size.y));   // fixed size

        // Begin window with no move, no resize
        ImGui::Begin("Register File",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse
        );

        ImVec2 size = ImGui::GetContentRegionAvail(); 
        ImGui::BeginChild("ASMView", size, true,
            ImGuiWindowFlags_HorizontalScrollbar);
        // buttons for value representation
        static bool hex_ver = false;
        static bool signed_ver = false;
        if (ImGui::SmallButton("Signed"))
            signed_ver = !signed_ver;

        ImGui::SameLine();
        if (ImGui::SmallButton("Hex"))
            hex_ver = !hex_ver;


        if (ImGui::BeginTable("regfile", 3)) {

            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Number");
            if(!signed_ver)
                ImGui::TableSetupColumn("Value(Unsigned)");
            else
                ImGui::TableSetupColumn("Value(Signed)");
            ImGui::TableHeadersRow(); 

            for (reg_id_t i = 0; i < 32;i++) {


                auto& data = _reg_file->at(i);
                std::stringstream representation;
                if (hex_ver && signed_ver)
                    representation << "0x" << std::hex << data._signed;
                else if (hex_ver && !signed_ver)
                    representation << "0x" << std::hex << data._unsigned;
                else if (signed_ver)
                    representation << data._signed;
                else
                    representation << data._unsigned;

                ImGui::TableNextRow();
                // register name
                ImGui::TableNextColumn();
                ImGui::Text(lookup_t::reg_name(i).c_str());

                // register number
                ImGui::TableNextColumn();
                ImGui::Text(std::to_string(i).c_str());

                // register value
                ImGui::TableNextColumn();
                ImGui::Text(representation.str().c_str());

            }
        }
        ImGui::EndTable();
        ImGui::EndChild();
        ImGui::End();
    }
}