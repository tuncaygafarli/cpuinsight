#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "../aliases.h"
#include "../cpu/cpu.h"

#include <vector>
#include <string>
enum class CPU_EXECUTION {
	RUN,
	STEP,
	RESET,
	STOP
};
namespace gui {
	void draw(const CPU& cpu, const std::vector<std::string>& assembly, bool& follow_pc,CPU_EXECUTION& action,float& cpu_speed);
}
