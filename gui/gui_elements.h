#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "../aliases.h"
#include "../cpu/cpu.h"
#include "gui_renderer.h"

#include <vector>
#include <string>

#include <fstream>
#include <iostream>
#include <chrono>

enum class CPU_EXECUTION {
	RUN,
	STEP,
	RESET,
	STOP
};
namespace gui {
	void draw(const CPU& cpu, const std::vector<std::string>& assembly, bool& follow_pc,CPU_EXECUTION& action,float& cpu_speed);

	template<typename T>
	void run(CPU& cpu,const T& parse_result) {

		gui::renderer::init("BranchPredict", 800, 600);
		GLFWwindow* window = gui::renderer::get_window();

		if (!window)
			return ;

		float cpu_speed = 0;
		CPU_EXECUTION action = CPU_EXECUTION::STOP;
		bool follow_pc = false;
		double accumulator = 0.0;
		auto last_time = std::chrono::high_resolution_clock::now();
		const double MAX_IPS = 100.0; 


		while (!glfwWindowShouldClose(window)) {
			gui::renderer::new_frame();
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize(gui::renderer::get_window_size());
			if (ImGui::Begin("##cpu_visual", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar)) {

				gui::draw(cpu, parse_result.second, follow_pc, action, cpu_speed);
				auto now = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> delta = now - last_time;
				last_time = now;

				switch (action) {
				case CPU_EXECUTION::RUN:
					if (!cpu.endofprogram()) {
						accumulator += cpu_speed * MAX_IPS * delta.count();
						while (accumulator >= 1.0 && !cpu.endofprogram()) {
							cpu.execute();
							accumulator -= 1.0;
						}
					}
					else {
						accumulator = 0;
					}
					break;
				case CPU_EXECUTION::STEP:
					if (!cpu.endofprogram())
						cpu.execute();
					action = CPU_EXECUTION::STOP;
					break;
				case CPU_EXECUTION::RESET:
					cpu.reset();
					break;
				case CPU_EXECUTION::STOP:
				default:
					break;

				}
			}
			ImGui::End();
			gui::renderer::render_frame(window);
		}

		gui::renderer::cleanup(window);
	}
}
