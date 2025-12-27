#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "../cpu/cpu.h";

class GUIRender {
private:
	struct instruction_element_t {
		sf::Color bg_color;
		std::string CODE;
		bool selected = false;

		instruction_element_t(sf::Color color, const std::string& code, bool sel)
			: bg_color(color), CODE(code), selected(sel) {}
	};

	struct reg_element_t {
		sf::Color bg_color;
		std::string REG_ID;
		std::string REG_DATA;

		reg_element_t(sf::Color color, const std::string& reg_id, const std::string& reg_data)
			: bg_color(color), REG_ID(reg_id), REG_DATA(reg_data) {
		}
	};

	sf::Font font;
	bool font_loaded = false;

	float register_scroll_offset = 0.f;
	int visible_registers_count = 12;
	float register_row_height = 0.f;

public :
	GUIRender();
	void init(CPU& cpu);
	void draw_gui(sf::RenderWindow& window, CPU& cpu);
	void draw_instructions(sf::RenderWindow& window);
	void draw_reg_file(sf::RenderWindow& window, CPU& cpu);
	void add_instruction(const std::string& asm_code);
	void set_selection(int& selectionIndex);
	void scroll(float amount);
	void scroll_registers(float amount);
	void ensure_visible(int index);
	void ensure_register_visible(int reg_index);
	void update_registers(CPU& cpu);


	float scroll_offset = 0.f;
	float visible_height = 0.f;

	std::vector<std::string> instruction_codes;
	std::vector<instruction_element_t> instruction_elements;
	std::vector<reg_element_t> reg_elements;
};