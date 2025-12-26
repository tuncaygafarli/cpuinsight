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

constexpr float HEADER_HEIGHT = 60.f;

GUIRender::GUIRender() {
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

void GUIRender::draw_gui(sf::RenderWindow& window, CPU& cpu) {
	draw_instructions(window);
	draw_reg_file(window, cpu);
}

void GUIRender::draw_instructions(sf::RenderWindow& window) {
	visible_height = static_cast<float>(window.getSize().y / 2);

	// Position for instructor panel ( left side )
	float instruction_start_x = 0.f;
	float instruction_start_y = HEADER_HEIGHT - scroll_offset;  // Start below header
	float instruction_box_width = window.getSize().x / 2;
	float content_height = visible_height - HEADER_HEIGHT;
	float instruction_box_height = content_height / 8;


	float total_content_height = instruction_elements.size() * instruction_box_height;
	float max_scroll = std::max(0.f, total_content_height - content_height);
	scroll_offset = std::clamp(scroll_offset, 0.f, max_scroll);

	sf::Vector2f instructor_size(instruction_box_width, instruction_box_height);

	for (int i = 0; i < instruction_elements.size(); i++) {
		float instruction_y_pos = instruction_start_y + i * instruction_box_height;

		if (instruction_y_pos + instruction_box_height < HEADER_HEIGHT) continue;
		if (instruction_y_pos > visible_height) break;

		// Instructor box start
		sf::RectangleShape instruction_panel(instructor_size);
		instruction_panel.setPosition(instruction_start_x, instruction_y_pos);

		if (instruction_elements[i].selected) {
			instruction_panel.setFillColor(sf::Color::White);
		}
		else {
			instruction_panel.setFillColor(instruction_elements[i].bg_color);
		}

		instruction_panel.setOutlineThickness(2.f);
		window.draw(instruction_panel);
		// Instructor box end

		// Instructor text start
		sf::Text instructor_text;
		instructor_text.setFont(font);
		instructor_text.setString(Helpers::trim_instruction(instruction_elements[i].CODE));
		instructor_text.setCharacterSize(24);
		instructor_text.setPosition(instruction_panel.getSize().x / 2, instruction_panel.getSize().y / 2 - instruction_box_height / 2);
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

	// Header for panel
	float header_start_x = 0.f;
	float header_start_y = 0.f;
	float header_box_width = window.getSize().x / 2;
	float header_box_height = HEADER_HEIGHT;
	sf::Vector2f header_size(header_box_width, header_box_height);

	// Header box
	sf::RectangleShape header_panel(header_size);
	header_panel.setPosition(header_start_x, header_start_y);
	header_panel.setFillColor(sf::Color::White);
	header_panel.setOutlineThickness(2.f);

	window.draw(header_panel);
	// Header box end

	// Header text start
	sf::Text header_text;
	header_text.setFont(font);
	header_text.setString("ASSEMBLY");
	header_text.setCharacterSize(24);
	header_text.setPosition(header_panel.getSize().x / 2, header_panel.getSize().y / 2 - header_box_height / 2);
	header_text.setFillColor(sf::Color::Black);

	sf::FloatRect header_text_bounds = header_text.getLocalBounds();
	float header_text_x = header_start_x + (header_box_width - header_text_bounds.width) / 2.f;
	float header_text_y = header_start_y + (header_box_height - header_text_bounds.height) / 2.f;

	header_text.setPosition(header_text_x, header_text_y);
	window.draw(header_text);
	// Header text end
}

void GUIRender::draw_reg_file(sf::RenderWindow& window, CPU& cpu) {
	register_row_height = window.getSize().y / 20.f;
	visible_registers_count = static_cast<int>((window.getSize().y - register_row_height) / register_row_height);
	float REG_CONST_HEIGHT = window.getSize().y / 20;

	// Position for register ID panel (right side)
	float reg_id_panel_x = window.getSize().x / 2;
	float reg_id_panel_y = REG_CONST_HEIGHT - register_scroll_offset;
	float reg_id_panel_width = window.getSize().x / 4;
	float reg_id_panel_height = window.getSize().y / 20;
	sf::Vector2f reg_id_panel_size(reg_id_panel_width, reg_id_panel_height);

	// Position for register DATA panel (right side)
	float reg_data_panel_x = 3 * window.getSize().x / 4;
	float reg_data_panel_y = REG_CONST_HEIGHT - register_scroll_offset;
	float reg_data_panel_width = window.getSize().x / 4;
	float reg_data_panel_height = window.getSize().y / 20;
	sf::Vector2f reg_data_panel_size(reg_data_panel_width, reg_data_panel_height);

	// calculate bounds from here
	float total_registers_height = reg_elements.size() * register_row_height;
	float max_register_scroll = std::max(0.f, total_registers_height -
		(window.getSize().y - register_row_height));
	register_scroll_offset = std::clamp(register_scroll_offset, 0.f, max_register_scroll);

	for (int i = 0; i < reg_elements.size(); i++) {
		float reg_id_y_pos = reg_id_panel_y + i * reg_id_panel_height;
		float reg_data_y_pos = reg_data_panel_y + i * reg_data_panel_height;

		if (reg_id_y_pos + reg_id_panel_height < register_row_height) continue;
		if (reg_id_y_pos > window.getSize().y) break;

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

	// Header for reg id panel
	float reg_id_header_start_x = window.getSize().x / 2;
	float reg_id_header_start_y = 0.f;
	float reg_id_header_box_width = window.getSize().x / 4;
	float reg_id_header_box_height = REG_CONST_HEIGHT;
	sf::Vector2f reg_id_header_size(reg_id_header_box_width, reg_id_header_box_height);

	// Header for reg id box
	sf::RectangleShape header_panel(reg_id_header_size);
	header_panel.setPosition(reg_id_header_start_x, reg_id_header_start_y);
	header_panel.setFillColor(sf::Color::White);
	header_panel.setOutlineThickness(2.f);

	window.draw(header_panel);
	// Header for reg id box end

	// Header for reg id text start
	sf::Text reg_id_header_text;
	reg_id_header_text.setFont(font);
	reg_id_header_text.setString("REG_ID");
	reg_id_header_text.setCharacterSize(24);
	reg_id_header_text.setPosition(header_panel.getSize().x / 2, header_panel.getSize().y / 2 - reg_id_header_box_height / 2);
	reg_id_header_text.setFillColor(sf::Color::Black);

	sf::FloatRect reg_id_header_text_bounds = reg_id_header_text.getLocalBounds();
	float header_text_x = reg_id_header_start_x + (reg_id_header_box_width - reg_id_header_text_bounds.width) / 2.f;
	float header_text_y = reg_id_header_start_y + (reg_id_header_box_height - reg_id_header_text_bounds.height) / 2.f;

	reg_id_header_text.setPosition(header_text_x, header_text_y);
	window.draw(reg_id_header_text);
	// Header for reg id text end

	// Header for reg data panel
	float reg_data_header_start_x = 3 * window.getSize().x / 4;
	float reg_data_header_start_y = 0.f;
	float reg_data_header_box_width = window.getSize().x / 4;
	float reg_data_header_box_height = window.getSize().y / 20;
	sf::Vector2f reg_data_header_size(reg_data_header_box_width, reg_data_header_box_height);

	// Header for reg id box
	sf::RectangleShape reg_data_header_panel(reg_data_header_size);
	reg_data_header_panel.setPosition(reg_data_header_start_x, reg_data_header_start_y);
	reg_data_header_panel.setFillColor(sf::Color::White);
	reg_data_header_panel.setOutlineThickness(2.f);

	window.draw(reg_data_header_panel);
	// Header for reg id box end

	// Header for reg id text start
	sf::Text reg_data_header_text;
	reg_data_header_text.setFont(font);
	reg_data_header_text.setString("REG_DATA");
	reg_data_header_text.setCharacterSize(24);
	reg_data_header_text.setPosition(reg_data_header_panel.getSize().x / 2, reg_data_header_panel.getSize().y / 2 - reg_data_header_box_height / 2);
	reg_data_header_text.setFillColor(sf::Color::Black);

	sf::FloatRect reg_data_header_text_bounds = reg_data_header_text.getLocalBounds();
	float reg_data_header_text_x = reg_data_header_start_x + (reg_data_header_box_width - reg_data_header_text_bounds.width) / 2.f;
	float reg_data_header_text_y = reg_data_header_start_y + (reg_data_header_box_height - reg_data_header_text_bounds.height) / 2.f;

	reg_data_header_text.setPosition(reg_data_header_text_x, reg_data_header_text_y);
	window.draw(reg_data_header_text);
	// Header for reg id text end
}


void GUIRender::add_instruction(const std::string& asm_code) {
	this->instruction_elements.emplace_back(
		sf::Color(45, 45, 50),
		asm_code,
		false
	);
}

void GUIRender::set_selection(int& selectionIndex) {
	for (auto& elem : instruction_elements) {
		elem.selected = false;
	}

	if (selectionIndex >= 0 && selectionIndex <static_cast<int>(instruction_elements.size())) {
		instruction_elements[selectionIndex].selected = true;
	}
}

void GUIRender::scroll(float amount) {
	const float HEADER_HEIGHT = 60.f;
	const float content_height = visible_height - HEADER_HEIGHT;
	const float INSTRUCTION_HEIGHT = content_height / 8;

	scroll_offset += amount;

	float total_content_height = instruction_elements.size() * INSTRUCTION_HEIGHT;
	float max_scroll = std::max(0.f, total_content_height - content_height);

	scroll_offset = std::clamp(scroll_offset, 0.f, max_scroll);
}

void GUIRender::ensure_visible(int index) {
	if (index < 0 || index >= static_cast<int>(instruction_elements.size())) return;

	const float HEADER_HEIGHT = 60.f;
	const float content_height = visible_height - HEADER_HEIGHT;
	const float INSTRUCTION_HEIGHT = content_height / 8;

	float item_top = index * INSTRUCTION_HEIGHT;
	float item_bottom = item_top + INSTRUCTION_HEIGHT;

	if (item_top < scroll_offset) {
		scroll_offset = item_top;
	}
	else if (item_bottom > scroll_offset + content_height) {
		scroll_offset = item_bottom - content_height;
	}

	float total_content_height = instruction_elements.size() * INSTRUCTION_HEIGHT;
	float max_scroll = std::max(0.f, total_content_height - content_height);
	scroll_offset = std::clamp(scroll_offset, 0.f, max_scroll);
}

void GUIRender::scroll_registers(float amount) {
	register_scroll_offset += amount;

	// Calculate bounds
	float total_registers_height = reg_elements.size() * register_row_height;
	float max_scroll = std::max(0.f, total_registers_height -
		(visible_height * 2 - register_row_height));  // Full window height

	register_scroll_offset = std::clamp(register_scroll_offset, 0.f, max_scroll);
}

void GUIRender::ensure_register_visible(int reg_index) {
	if (reg_index < 0 || reg_index >= static_cast<int>(reg_elements.size())) return;

	float item_top = reg_index * register_row_height;
	float item_bottom = item_top + register_row_height;
	float visible_area_height = visible_height * 2 - register_row_height;

	if (item_top < register_scroll_offset) {
		register_scroll_offset = item_top;
	}
	else if (item_bottom > register_scroll_offset + visible_area_height) {
		register_scroll_offset = item_bottom - visible_area_height;
	}

	float total_registers_height = reg_elements.size() * register_row_height;
	float max_scroll = std::max(0.f, total_registers_height - visible_area_height);
	register_scroll_offset = std::clamp(register_scroll_offset, 0.f, max_scroll);
}