#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class instruction_box_t {
private:
	struct instruction_element_t {
		sf::Color bg_color;
		std::string CODE;
		bool selected = false;

		instruction_element_t(sf::Color color, const std::string& code, bool sel)
			: bg_color(color), CODE(code), selected(sel) {}
	};
	sf::Font font;
	bool font_loaded = false;

public :
	instruction_box_t();
	void draw_instructions(sf::RenderWindow& window);
	void add_instruction(const std::string& asm_code);
	void set_selection(int& selectionIndex);

	std::vector<std::string> instruction_codes = {"MOV EAX, 10", "MOV [EBX], EAX", "MOV EAX, 10", "MOV [EBX], EAX","MOV EAX, 10", "MOV [EBX], EAX" };
	std::vector<instruction_element_t> instruction_elements;
};