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
constexpr float LOGGER_HEIGHT = 20.f;
constexpr float MARGIN = 5.f;

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

void GUIRender::update_instructions(CPU& cpu) {
	instruction_elements.clear();
	for (size_t i = 0; i < instruction_codes.size(); i++) {
		instruction_elements.emplace_back(
			sf::Color(35, 35, 40),
			instruction_codes[i],
			i == 0
		);
	}
}

void GUIRender::draw_gui(sf::RenderWindow& window, CPU& cpu) {
	draw_instructions(window);
	draw_reg_file(window, cpu);

	if (logger_enabled) {
		draw_prompt(window, cpu);
	}
}

void GUIRender::draw_box(sf::RenderWindow& window,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Color& bg_color,
	const std::string& text,
	const sf::Color& text_color,
	unsigned int text_size,
	bool center_text) {

	sf::RectangleShape box(size);
	box.setPosition(position);
	box.setFillColor(bg_color);
	box.setOutlineThickness(2.f);
	window.draw(box);

	if (!text.empty()) {
		sf::Text text_obj;
		text_obj.setFont(font);
		text_obj.setString(text);
		text_obj.setCharacterSize(text_size);
		text_obj.setFillColor(text_color);

		if (center_text) {
			sf::FloatRect textBounds = text_obj.getLocalBounds();
			float text_x = position.x + (size.x - textBounds.width) / 2.f;
			float text_y = position.y + (size.y - textBounds.height) / 2.f;
			text_obj.setPosition(text_x, text_y);
		}
		else {
			text_obj.setPosition(position.x + 10,
				position.y + size.y / 2);
		}

		window.draw(text_obj);
	}
}

void GUIRender::draw_instructions(sf::RenderWindow& window) {
	visible_height = static_cast<float>(window.getSize().y / 2);

	// Position for instructor panel
	float instruction_start_x = 0.f;
	float instruction_start_y = HEADER_HEIGHT - scroll_offset;
	float instruction_box_width = window.getSize().x / 2;
	float content_height = visible_height - HEADER_HEIGHT;
	float instruction_box_height = content_height / 8;

	float total_content_height = instruction_elements.size() * instruction_box_height;
	float max_scroll = std::max(0.f, total_content_height - content_height);
	scroll_offset = std::clamp(scroll_offset, 0.f, max_scroll);

	for (int i = 0; i < instruction_elements.size(); i++) {
		float instruction_y_pos = instruction_start_y + i * instruction_box_height;

		if (instruction_y_pos + instruction_box_height < HEADER_HEIGHT) continue;
		if (instruction_y_pos > visible_height) break;

		sf::Color bg_color = instruction_elements[i].selected ?
			sf::Color::White :
			instruction_elements[i].bg_color;

		sf::Color text_color = instruction_elements[i].selected ?
			sf::Color::Black :
			sf::Color::White;

		draw_box(window,
			sf::Vector2f(instruction_start_x, instruction_y_pos),
			sf::Vector2f(instruction_box_width, instruction_box_height),
			bg_color,
			Helpers::trim_instruction(instruction_elements[i].CODE),
			text_color);
	}

	draw_box(window,
		sf::Vector2f(0.f, 0.f),
		sf::Vector2f(window.getSize().x / 2, HEADER_HEIGHT),
		sf::Color::White,
		"ASSEMBLY",
		sf::Color::Black,
		24,
		true);

}

void GUIRender::draw_reg_file(sf::RenderWindow& window, CPU& cpu) {
	register_row_height = window.getSize().y / 20.f;
	visible_registers_count = static_cast<int>((window.getSize().y - register_row_height) / register_row_height);
	float REG_CONST_HEIGHT = window.getSize().y / 20;


	// register positions
	float reg_id_panel_x = window.getSize().x / 2;
	float reg_id_panel_y = REG_CONST_HEIGHT - register_scroll_offset;
	float reg_id_panel_width = window.getSize().x / 4;
	float reg_id_panel_height = window.getSize().y / 20;

	float reg_data_panel_x = 3 * window.getSize().x / 4;
	float reg_data_panel_y = REG_CONST_HEIGHT - register_scroll_offset;
	float reg_data_panel_width = window.getSize().x / 4;
	float reg_data_panel_height = window.getSize().y / 20;

	// scroll bounds
	float total_registers_height = reg_elements.size() * register_row_height;
	float max_register_scroll = std::max(0.f, total_registers_height -
		(window.getSize().y - register_row_height));
	register_scroll_offset = std::clamp(register_scroll_offset, 0.f, max_register_scroll);

	// registers
	for (int i = 0; i < reg_elements.size(); i++) {
		float reg_id_y_pos = reg_id_panel_y + i * reg_id_panel_height;
		float reg_data_y_pos = reg_data_panel_y + i * reg_data_panel_height;

		if (reg_id_y_pos + reg_id_panel_height < register_row_height) continue;
		if (reg_id_y_pos > window.getSize().y) break;

		// register ID box
		draw_box(window,
			sf::Vector2f(reg_id_panel_x, reg_id_y_pos),
			sf::Vector2f(reg_id_panel_width, reg_id_panel_height),
			reg_elements[i].bg_color,
			reg_elements[i].REG_ID,
			sf::Color::White);

		// register DATA box
		draw_box(window,
			sf::Vector2f(reg_data_panel_x, reg_data_y_pos),
			sf::Vector2f(reg_data_panel_width, reg_data_panel_height),
			reg_elements[i].bg_color,
			reg_elements[i].REG_DATA,
			sf::Color::White);
	}

	// REG_ID header
	draw_box(window,
		sf::Vector2f(window.getSize().x / 2, 0.f),
		sf::Vector2f(window.getSize().x / 4, HEADER_HEIGHT),
		sf::Color::White,
		"REG_ID",
		sf::Color::Black,
		24,
		true);

	// REG_DATA header
	draw_box(window,
		sf::Vector2f(3 * window.getSize().x / 4, 0.f),
		sf::Vector2f(window.getSize().x / 4, HEADER_HEIGHT),
		sf::Color::White,
		"REG_DATA",
		sf::Color::Black,
		24,
		true);
}

void GUIRender::draw_prompt(sf::RenderWindow& window, CPU& cpu) {
	// logger input field
	float logger_panel_width = window.getSize().x / 2 - 2.f;
	float logger_panel_height = 50.f;
	float logger_panel_x = 0.f;
	float logger_panel_y = window.getSize().y - logger_panel_height;

	draw_box(window,
		sf::Vector2f(logger_panel_x, logger_panel_y),
		sf::Vector2f(logger_panel_width, logger_panel_height),
		sf::Color::White,
		"",
		sf::Color::Black,
		24,
		true);

	float cursor_x = logger_panel_x + 10;
	float cursor_y = logger_panel_y + 10;

	if (!logger_text.empty()) {
		sf::Text text_obj;
		text_obj.setFont(font);
		text_obj.setString(logger_text);
		text_obj.setCharacterSize(24);
		text_obj.setFillColor(sf::Color::Black);
		text_obj.setPosition(logger_panel_x + 10, cursor_y);
		window.draw(text_obj);

		cursor_x = text_obj.getPosition().x + text_obj.getLocalBounds().width;
	}

	static sf::Clock cursor_clock;
	if (current_mode == InputMode::TEXT && static_cast<int>(cursor_clock.getElapsedTime().asSeconds() * 2) % 2 == 0) {
		sf::RectangleShape cursor(sf::Vector2f(8, 24));
		cursor.setFillColor(sf::Color::Black);
		cursor.setPosition(cursor_x, cursor_y + 2.f);
		window.draw(cursor);
	}
}

void GUIRender::set_text(sf::Uint32 unicode) {
	if (unicode >= 32 && unicode < 128) {
		logger_text += static_cast<char>(unicode);
	}
	else if (unicode == 8) {
		if (!logger_text.empty()) {
			logger_text.pop_back();
		}
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