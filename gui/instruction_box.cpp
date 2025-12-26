#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "instruction_box.h"

instruction_box_t::instruction_box_t(){
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

void instruction_box_t::draw_instructions(sf::RenderWindow& window) {
	sf::Vector2f size(window.getSize().x, 50.f);

	float startX = 0.f;
	float startY = 0.f;
	float boxWidth = window.getSize().x;
	float boxHeight = 40.f;  
	float spacing = 5.f;

	for (int i = 0; i < this->instruction_elements.size(); i++) {
		float yPos = startY + i * (boxHeight + spacing);
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

void instruction_box_t::add_instruction(const std::string& asm_code) {
	this->instruction_elements.emplace_back(
		sf::Color(45, 45, 50),
		asm_code,
		false
	);
}

void instruction_box_t::set_selection(int& selectionIndex) {
	for (auto& elem : this->instruction_elements) {
		elem.selected = false;
	}

	if (selectionIndex >= 0 && selectionIndex < static_cast<int>(this->instruction_elements.size())) {
		this->instruction_elements[selectionIndex].selected = true;
	}
}