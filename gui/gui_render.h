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

	enum class InputMode {
		NAVIGATION,
		TEXT
	};

	void init(CPU& cpu);
	void draw_gui(sf::RenderWindow& window, CPU& cpu);
	void draw_instructions(sf::RenderWindow& window);
	void draw_reg_file(sf::RenderWindow& window, CPU& cpu);
	void draw_logger(sf::RenderWindow& window, CPU& cpu);
	void draw_box(sf::RenderWindow& window,
		const sf::Vector2f& position,
		const sf::Vector2f& size,
		const sf::Color& bg_color,
		const std::string& text,
		const sf::Color& text_color,
		unsigned int text_size = 24,
		bool center_text = true);

	void add_instruction(const std::string& asm_code);
	void set_selection(int& selectionIndex);
	void scroll(float amount);
	void scroll_registers(float amount);
	void ensure_visible(int index);
	void ensure_register_visible(int reg_index);
	void update_registers(CPU& cpu);


	InputMode current_mode = InputMode::NAVIGATION;
	void set_mode(InputMode mode) { current_mode = mode; }

	// instructions props
	float scroll_offset = 0.f;
	float visible_height = 0.f;

	// logger props
	std::string logger_text;
	void set_text(sf::Uint32 unicode);
	bool logger_enabled = false;

	std::vector<std::string> instruction_codes;
	std::vector<instruction_element_t> instruction_elements;
	std::vector<reg_element_t> reg_elements;
};