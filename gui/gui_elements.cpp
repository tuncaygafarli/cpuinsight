
#include "gui_elements.h"
#include "../parser/lookup.h"

#include <cmath>
#include <sstream>
namespace gui {
	namespace {
		void draw_toolbar(CPU_EXECUTION& action, float& speed, bool& follow_pc) {

			ImGui::SetNextItemWidth(150.0f); 
			ImGui::SliderFloat("Speed", &speed, 0.0f, 1.0f, "%.2f");
			if (ImGui::Button("Run"))
				action = CPU_EXECUTION::RUN;
			ImGui::SameLine();
			if (ImGui::Button("Step"))
				action = CPU_EXECUTION::STEP;
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
				action = CPU_EXECUTION::STOP;
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
				action = CPU_EXECUTION::RESET;
			ImGui::SameLine();
			if (follow_pc) {
				if (ImGui::Button("Unfollow PC")) {
					follow_pc = !follow_pc;
				}
			}
			else {
				if (ImGui::Button("Follow PC")) {
					follow_pc = !follow_pc;
				}

			}
			ImGui::Separator();
		}

		void draw_assembly(const std::vector<std::string>& assembly, const CPU& cpu, const bool& follow_pc,const ImVec2& size) {

			auto& pc = cpu.get_pc();
			if (
				ImGui::BeginChild(
					"ASMView",
					size,
					true,
					ImGuiWindowFlags_HorizontalScrollbar
				)
				) {
				if (follow_pc) {
					float line_height = ImGui::GetTextLineHeightWithSpacing();
					float window_height = ImGui::GetWindowHeight();

					float instruction_pos = static_cast<float>(pc) * line_height;

					float scroll_goal = instruction_pos - (window_height * 0.5f);

					ImGui::SetScrollY(scroll_goal);
				}
				ImGuiListClipper clipper;
				clipper.Begin(static_cast<int32_t>(assembly.size()));
				while (clipper.Step()) {
					for (int32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
						const bool is_pc = (i == pc);
						if (is_pc) {
							ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
							ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(255, 100, 100, 60));
							ImGui::Selectable((std::to_string(i + 1) + ". " + assembly[i]).c_str(), true,
								ImGuiSelectableFlags_SpanAllColumns);
							ImGui::PopStyleColor(2);
						}
						else {
							ImGui::TextUnformatted((std::to_string(i + 1) + ". " + assembly[i]).c_str());
						}
					}
				}

			}
			ImGui::EndChild();

		}


		void draw_performance_data(const CPU& cpu,const ImVec2& size) {

			auto window_size = ImGui::GetWindowSize();
			if (ImGui::BeginChild("Performance", size, true)) {
				if (ImGui::BeginTable("PerfTable", 3, ImGuiTableFlags_SizingStretchProp)) {

					ImGui::TableNextColumn();
					ImGui::Text("Cycles: %llu", cpu.get_cycles());

					ImGui::TableNextColumn();
					ImGui::Text("Branches: %u", cpu.get_total_branches());

					ImGui::TableNextColumn();
					float accuracy = cpu.get_total_branches() > 0
						? (static_cast<float>(cpu.get_correct_predictions()) / cpu.get_total_branches()) * 100.0f
						: 0.0f;

					ImGui::Text("Correct: %u (%.1f%%)", cpu.get_correct_predictions(), accuracy);

				}
				ImGui::EndTable();
			}
			ImGui::EndChild();
		}
		void draw_register_file(const CPU& cpu, ImVec2 size) {
			if (ImGui::BeginChild("regchild", size, true)) {
				static bool hex_ver = false;
				static bool signed_ver = false;

				if (signed_ver) {
					if (ImGui::SmallButton("Switch to Unsigned")) signed_ver = false;
				} else {
					if (ImGui::SmallButton("Switch to Signed")) signed_ver = true;
				}

				ImGui::SameLine();

				if (hex_ver) {
					if (ImGui::SmallButton("Switch to Decimal")) hex_ver = false;
				} else {
					if (ImGui::SmallButton("Switch to Hex")) hex_ver = true;
				}
			

				if (ImGui::BeginTable("regfile", 3, ImGuiTableFlags_ScrollY)) {
					ImGui::TableSetupScrollFreeze(0, 1); 
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("No.");
					ImGui::TableSetupColumn(signed_ver ? "Value (S)" : "Value (U)");
					ImGui::TableHeadersRow();

					for (uint8_t i = 0; i < 32; i++) {
						auto& data = cpu.get_reg_file().at(i);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::TextUnformatted(lookup_t::reg_name(i).c_str());
						
						ImGui::TableNextColumn();
						ImGui::Text("%d", i);

						ImGui::TableNextColumn();
						if (hex_ver) 
							ImGui::Text("0x%X", signed_ver ? data._signed : data._unsigned);
						else 
							ImGui::Text(signed_ver ? "%lld" : "%llu", signed_ver ? data._signed : data._unsigned);
					}
				}
				ImGui::EndTable();
			}
			ImGui::EndChild();
		}
		void draw_dcache(const CPU& cpu, const ImVec2& size) {
			if (ImGui::BeginChild("##datacache", size, true)) {


				static bool signed_ver = false;
				static bool hex_ver = false;
				if (signed_ver) {
					if (ImGui::SmallButton("Switch to Unsigned")) signed_ver = false;
				} else {
					if (ImGui::SmallButton("Switch to Signed")) signed_ver = true;
				}

				ImGui::SameLine();

				if (hex_ver) {
					if (ImGui::SmallButton("Switch to Decimal")) hex_ver = false;
				} else {
					if (ImGui::SmallButton("Switch to Hex")) hex_ver = true;
				}
				
				if (ImGui::BeginTable("dcache", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerH)) {
					
					ImGui::TableSetupScrollFreeze(0, 1); 
					ImGui::TableSetupColumn("Address");
					ImGui::TableSetupColumn("Data");
					ImGui::TableHeadersRow();

					for (auto& [addr, data] : cpu.get_dcache()) {
						ImGui::TableNextRow();
						
						ImGui::TableNextColumn();
						ImGui::Text("0x%08X", addr); 

						ImGui::TableNextColumn();
						if (hex_ver) 
								ImGui::Text("0x%X", signed_ver ? data._signed : data._unsigned);
							else 
								ImGui::Text(signed_ver ? "%lld" : "%llu", signed_ver ? data._signed : data._unsigned);
					}
				}
				ImGui::EndTable();
			}
			ImGui::EndChild();
		}
	
	} // anonymous
	void draw(const CPU& cpu, const std::vector<std::string>& assembly, bool& follow_pc, CPU_EXECUTION& action, float& speed) {
		float footer_height = 100.0f; 
		ImVec2 available_space = ImGui::GetContentRegionAvail();
		
		float left_column_width = available_space.x * 0.66f; 
		float right_column_width = available_space.x - left_column_width - ImGui::GetStyle().ItemSpacing.x;
		
		float main_content_height = available_space.y - footer_height - ImGui::GetStyle().ItemSpacing.y;

		ImGui::BeginGroup();
			draw_toolbar(action, speed, follow_pc);
			ImGui::Text("Assembly");
			draw_assembly(assembly, cpu, follow_pc, ImVec2(left_column_width, main_content_height - ImGui::GetFrameHeightWithSpacing()));
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
			float right_item_height = (main_content_height / 2.0f) - ImGui::GetFrameHeightWithSpacing();

			ImGui::Text("Register File");
			draw_register_file(cpu, ImVec2(right_column_width, right_item_height));

			ImGui::Text("Cache");
			draw_dcache(cpu, ImVec2(right_column_width, right_item_height));
		ImGui::EndGroup();

		draw_performance_data(cpu, ImVec2(available_space.x, footer_height));
	}

} // namespace gui
