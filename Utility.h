#pragma once

#include <SFML/Graphics.hpp>

const sf::Color empty = sf::Color::White;
const sf::Color wall = sf::Color::Black;
const sf::Color way = sf::Color::Red;
const sf::Color finish = sf::Color::Green;

struct Utility
{
	Utility(const std::string& fileName, sf::RenderWindow& window);
	~Utility();

	sf::Image& image();
	void update();
	void draw();

private:
	sf::RenderWindow& window_;
	sf::Image image_;
	sf::Texture texture_;
	sf::Rect<int> renderRectangle_;
	sf::Sprite sprite_;
};

