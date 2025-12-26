#include <SFML/Graphics.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

#include "gui_render.h"

GUIRender::GUIRender(){
	for (size_t i = 0; i < instruction_codes.size(); i++) {
		instruction_elements.emplace_back(
			sf::Color(45, 45, 50),
			instruction_codes[i],
			i == 0
		);
	}

	std::string font_path = "C:\\Users\\Admin\\Downloads\\BigBlueTerminal\\BigBlueTermPlusNerdFontMono-Regular.ttf";

	if (!font.loadFromFile(font_path))
	{
		std::cerr << "Failed to load font." << "\n";
	}
}

void GUIRender::draw_gui(sf::RenderWindow& window) {
	sf::Vector2f size(window.getSize().x / 2, 50.f);
	visible_height = static_cast<float>(window.getSize().y);

	float startX = 0.f;
	float startY = 0.f - scroll_offset;
	float boxWidth = window.getSize().x / 2;
	float boxHeight = 40.f;  
	float spacing = 5.f;

	float total_height = this->instruction_elements.size() * (boxHeight + spacing);

	float max_scroll = std::max(0.f, total_height - visible_height);
	scroll_offset = std::clamp(scroll_offset, 0.f, max_scroll);

	for (int i = 0; i < this->instruction_elements.size(); i++) {
		float yPos = startY + i * (boxHeight + spacing);

		if (yPos + boxHeight < 0) continue;

		if (yPos > visible_height) break;
		
		// Element box
		sf::RectangleShape box(size);
		box.setPosition(startX, yPos);

		if (this->instruction_elements[i].selected) {
			box.setFillColor(sf::Color::White);
		}
		else {
			box.setFillColor(this->instruction_elements[i].bg_color);
		}

		box.setOutlineThickness(2.f);
		window.draw(box);

		// Element text
		sf::Text text;
		text.setFont(font);
		text.setString(this->instruction_elements[i].CODE);
		text.setCharacterSize(24);
		text.setPosition(box.getSize().x / 2, box.getSize().y / 2);
		if (this->instruction_elements[i].selected) {
			text.setFillColor(sf::Color::Black);
		}
		else {
			text.setFillColor(sf::Color::White);
		}

		sf::FloatRect textBounds = text.getLocalBounds();
		float textX = startX + (boxWidth - textBounds.width) / 2.f;
		float textY = yPos + (boxHeight - textBounds.height) / 2.f;

		text.setPosition(textX, textY);
		window.draw(text);
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

	float total_height = instruction_elements.size() * (45.f); // boxHeight + spacing
	float max_scroll = std::max(0.f, total_height - visible_height);

	scroll_offset = std::clamp(scroll_offset, 0.f, max_scroll);
}

void GUIRender::ensure_visible(int index) {
	if (index < 0 || index >= static_cast<int>(instruction_elements.size())) return;

	float boxHeight = 40.f;
	float spacing = 5.f;
	float item_height = boxHeight + spacing;

	float item_top = index * item_height;
	float item_bottom = item_top + item_height;

	if (item_top < scroll_offset) {
		scroll_offset = item_top;
	}
	else if (item_bottom > scroll_offset + visible_height) {
		scroll_offset = item_bottom - visible_height;
	}
}