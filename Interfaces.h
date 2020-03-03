#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>

using Point = sf::Vector2<int>;

const auto Empty = sf::Color::White;
const auto Wall = sf::Color::Black;

struct IAlgorithm
{
	virtual std::vector<Point> findPath(std::vector<std::vector<sf::Color>> bitmap, Point start, Point end) = 0;
	// virtual std::vector<Point> findPath(sf::Image& bitmap, Point start, Point end) = 0;
};

struct IVisualisedAlgorithm
{
	virtual void setTarget(sf::Image& bitmap, const Point start, const Point end) = 0;
	// mark 10 pixels that have been visited
	virtual bool step() = 0;
};

enum class AlgorithmType
{
	Simple,
	Visual
};

struct IEngine
{
	virtual void setAlgorithm(const AlgorithmType) = 0;
	virtual void act() = 0;
	virtual void handleMouse(int x, int y, int button) = 0;
	virtual void handleKeyboard(char key) = 0;
};
