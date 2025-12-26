#include <SFML/Graphics.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>

#include "gui_render.h"
#include "../cpu/cpu.h"
#include "helpers.h"

GUIRender::GUIRender(){
	std::string font_path = "C:\\Users\\Admin\\Downloads\\BigBlueTerminal\\BigBlueTermPlusNerdFontMono-Regular.ttf";

	if (!font.loadFromFile(font_path))
	{
		std::cerr << "Failed to load font." << "\n";
	}
}

// external initializer
void GUIRender::init(CPU& cpu) {
	const reg_file_t& reg_file = cpu.get_reg_file();

	for (size_t i = 0; i < instruction_codes.size(); i++) {
		instruction_elements.emplace_back(
			sf::Color(35, 35, 40),
			instruction_codes[i],
			i == 0
		);
	}

	for (const auto& reg_pair : reg_file) {
		const reg_id_t& reg_id = reg_pair.first;
		const data_t& reg_data = reg_pair.second;

		std::string reg_id_str = Helpers::id_t_to_string(reg_id);
		std::string reg_data_str = Helpers::data_t_to_string(reg_data);

		reg_elements.emplace_back(
			sf::Color(45, 45, 50),
			reg_id_str,
			reg_data_str
		);
	}
}

// updates register elements after every CPU execution because reg_file_t changes
void GUIRender::update_registers(CPU& cpu) {
	const reg_file_t& reg_file = cpu.get_reg_file();
	reg_elements.clear();

	for (const auto& reg_pair : reg_file) {
		const reg_id_t& reg_id = reg_pair.first;
		const data_t& reg_data = reg_pair.second;

		std::string reg_id_str = Helpers::id_t_to_string(reg_id);
		std::string reg_data_str = Helpers::data_t_to_string(reg_data);

		reg_elements.emplace_back(
			sf::Color(45, 45, 50),
			reg_id_str,
			reg_data_str
		);
	}
}



void GUIRender::draw_gui(sf::RenderWindow& window, CPU& cpu){
	draw_instructions(window);
	draw_reg_file(window, cpu);
}

void GUIRender::draw_instructions(sf::RenderWindow& window) {
	visible_height = static_cast<float>(window.getSize().y / 2);

	// Position for instructor panel ( left side )
	float instruction_start_x = 0.f;
	float instruction_start_y = 0.f - scroll_offset;
	float instruction_box_width = window.getSize().x / 2;
	float instruction_box_height = visible_height / 8;

	float total_height = instruction_elements.size() * instruction_box_height;

	float max_scroll = std::max(0.f, total_height - visible_height);
	scroll_offset = std::clamp(scroll_offset, 0.f, max_scroll);

	sf::Vector2f instructor_size(instruction_box_width, instruction_box_height);

	for (int i = 0; i < instruction_elements.size(); i++) {
		float instruction_y_pos = instruction_start_y + i * instruction_box_height;

		if (instruction_y_pos + instruction_box_height < 0) continue;

		if (instruction_y_pos > visible_height) break;

		// Instructor box start
		sf::RectangleShape box(instructor_size);
		box.setPosition(instruction_start_x, instruction_y_pos);

		if (instruction_elements[i].selected) {
			box.setFillColor(sf::Color::White);
		}
		else {
			box.setFillColor(instruction_elements[i].bg_color);
		}

		box.setOutlineThickness(2.f);
		window.draw(box);
		// Instructor box end

		// Instructor text start
		sf::Text instructor_text;
		instructor_text.setFont(font);
		instructor_text.setString(Helpers::trim_instruction(instruction_elements[i].CODE));
		instructor_text.setCharacterSize(24);
		instructor_text.setPosition(box.getSize().x / 2, box.getSize().y / 2 - instruction_box_height / 2);
		if (instruction_elements[i].selected) {
			instructor_text.setFillColor(sf::Color::Black);
		}
		else {
			instructor_text.setFillColor(sf::Color::White);
		}

		sf::FloatRect textBounds = instructor_text.getLocalBounds();
		float instruction_text_x = instruction_start_x + (instruction_box_width - textBounds.width) / 2.f;
		float instruction_text_y = instruction_y_pos + (instruction_box_height - textBounds.height) / 2.f;

		instructor_text.setPosition(instruction_text_x, instruction_text_y);
		window.draw(instructor_text);
		// Instructor text end

	}
}

void GUIRender::draw_reg_file(sf::RenderWindow& window, CPU& cpu) {
	// Position for register ID panel (right side)
	float reg_id_panel_x = window.getSize().x / 2;
	float reg_id_panel_y = 0.f;
	float reg_id_panel_width = window.getSize().x / 4;
	float reg_id_panel_height = window.getSize().y / 20;
	sf::Vector2f reg_id_panel_size(reg_id_panel_width, reg_id_panel_height);

	// Position for register DATA panel (right side)
	float reg_data_panel_x = 3 * window.getSize().x / 4;
	float reg_data_panel_y = 0.f;
	float reg_data_panel_width = window.getSize().x / 4;
	float reg_data_panel_height = window.getSize().y / 20;
	sf::Vector2f reg_data_panel_size(reg_data_panel_width, reg_data_panel_height);

	for (int i = 0; i < reg_elements.size(); i++) {
		float reg_id_y_pos = reg_id_panel_y + i * reg_id_panel_height;
		float reg_data_y_pos = reg_data_panel_y + i * reg_data_panel_height;

		// Reg ID panel start
		sf::RectangleShape reg_id_panel(reg_id_panel_size);
		reg_id_panel.setPosition(reg_id_panel_x, reg_id_y_pos);

		reg_id_panel.setFillColor(reg_elements[i].bg_color);

		reg_id_panel.setOutlineThickness(2.f);
		window.draw(reg_id_panel);
		// Reg ID panel end

		// Reg ID text start
		sf::Text reg_id_text;
		reg_id_text.setFont(font);
		reg_id_text.setString(reg_elements[i].REG_ID);
		reg_id_text.setCharacterSize(24);
		reg_id_text.setPosition(reg_id_panel.getSize().x / 2, reg_id_panel.getSize().y / 2);
		reg_id_text.setFillColor(sf::Color::White);

		sf::FloatRect id_textBounds = reg_id_text.getLocalBounds();
		float id_textX = reg_id_panel_x + (reg_id_panel_width - id_textBounds.width) / 2.f;
		float id_textY = reg_id_y_pos + (reg_id_panel_height - id_textBounds.height) / 2.f;

		reg_id_text.setPosition(id_textX, id_textY);
		window.draw(reg_id_text);
		// Reg ID text end

		// Reg DATA panel start
		sf::RectangleShape reg_data_panel(reg_data_panel_size);
		reg_data_panel.setPosition(reg_data_panel_x, reg_data_y_pos);

		reg_data_panel.setFillColor(reg_elements[i].bg_color);

		reg_data_panel.setOutlineThickness(2.f);
		window.draw(reg_data_panel);
		// Reg DATA panel end

		// Reg DATA text start
		sf::Text reg_data_text;
		reg_data_text.setFont(font);
		reg_data_text.setString(reg_elements[i].REG_DATA);
		reg_data_text.setCharacterSize(24);
		reg_data_text.setPosition(reg_data_panel.getSize().x / 2, reg_data_panel.getSize().y / 2);
		reg_data_text.setFillColor(sf::Color::White);

		sf::FloatRect data_textBounds = reg_data_text.getLocalBounds();
		float data_textX = reg_data_panel_x + (reg_data_panel_width - data_textBounds.width) / 2.f;
		float data_textY = reg_data_y_pos + (reg_data_panel_height - data_textBounds.height) / 2.f;

		reg_data_text.setPosition(data_textX, data_textY);
		window.draw(reg_data_text);
		// Reg DATA text end
	}
}


void GUIRender::add_instruction(const std::string& asm_code) {
	this->instruction_elements.emplace_back(
		sf::Color(45, 45, 50),
		asm_code,
		false
	);
}

void GUIRender::set_selection(int& selectionIndex) {
	for (auto& elem : this->instruction_elements) {
		elem.selected = false;
	}

	if (selectionIndex >= 0 && selectionIndex < static_cast<int>(this->instruction_elements.size())) {
		this->instruction_elements[selectionIndex].selected = true;
	}
}

void GUIRender::scroll(float amount) {
	scroll_offset += amount;

	float total_height = instruction_elements.size() * (50.f);
	float max_scroll = std::max(0.f, total_height - visible_height);

	scroll_offset = std::clamp(scroll_offset, 0.f, max_scroll);
}

void GUIRender::ensure_visible(int index) {
	if (index < 0 || index >= static_cast<int>(instruction_elements.size())) return;

	float boxHeight = visible_height / 8;
	float item_height = boxHeight;

	float item_top = index * item_height;
	float item_bottom = item_top + item_height;

	if (item_top < scroll_offset) {
		scroll_offset = item_top;
	}
	else if (item_bottom > scroll_offset + visible_height) {
		scroll_offset = item_bottom - visible_height;
	}
}
