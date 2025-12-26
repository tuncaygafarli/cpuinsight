#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class GUIRender {
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

	float scroll_offset = 0.f; 
	float visible_height = 0.f;

public :
	GUIRender();
	void draw_gui(sf::RenderWindow& window);
	void add_instruction(const std::string& asm_code);
	void set_selection(int& selectionIndex);
	void scroll(float amount);
	void ensure_visible(int index);

	std::vector<std::string> instruction_codes = {
		 "MOV EAX, 10", "MOV [EBX], EAX", "ADD ECX, 5",
		"SUB EDX, 2", "CMP EAX, 0", "JNZ loop",
		"PUSH EAX", "POP EBX", "CALL func",
		"RET", "NOP", "HLT", "MOV ESI, 0",
		"INC ESI", "DEC EDI", "JMP start","MOV EAX, 10", "MOV [EBX], EAX", "ADD ECX, 5",
		"SUB EDX, 2", "CMP EAX, 0", "JNZ loop",
		"PUSH EAX", "POP EBX", "CALL func",
		"RET", "NOP", "HLT", "MOV ESI, 0",
		"INC ESI", "DEC EDI", "JMP start"
	};
	std::vector<instruction_element_t> instruction_elements;
};