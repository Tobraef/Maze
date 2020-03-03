#pragma once
#include <SFML/Graphics.hpp>

#include <vector>
#include <functional>

namespace utils
{
	struct Boi 
	{ 
		int x; int y; Boi(int x_, int y_) : x(x_), y(y_){} Boi() = default;
		bool operator < (const Boi& other) const
		{
			if (x == other.x)
				return y < other.y;
			return x < other.x;
		}
	};
	enum movement
	{
		G, D, P, L
	};
	const std::map<movement, std::function<void(Boi&)>> moveMap =
	{
		{ G, [](Boi& b) { b.y--; } },
		{ D, [](Boi& b) { b.y++; } },
		{ L, [](Boi& b) { b.x--; } },
		{ P, [](Boi& b) { b.x++; } }
	};
}

class Labirynth
{
public:
	Labirynth(const std::string& imFileName, sf::RenderWindow& w);
	virtual ~Labirynth() = default;

	void solveDaMaze(const std::pair<int,int>& start, const std::pair<int,int>& end);
	void solveDaMazeLikeABoss(const std::pair<int, int>& start, const std::pair<int, int>& end);
	void draw(sf::RenderWindow&);
private:
	std::pair<int, int> getTopLeftCorner(const sf::Vector2<unsigned>&) const;
	std::pair<int, int> getBottomRightCorner(const sf::Vector2<unsigned>&) const;
	void coverEdgesOfLabirynth(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2);
	void setPicture();

	void moveDueToCurrentPosition(utils::Boi& b, utils::movement&);
	bool checkIfCloseEnough(const utils::Boi b) const;
	void beginMovement(utils::Boi& b);
	void markExit(const utils::Boi& b);

	bool exquisiteSolver(const utils::Boi&, std::vector<std::vector<int>>&);
	bool checkIfFoundExit(const utils::Boi&);
	void markWayBack(const std::vector<std::vector<int>>&);
	utils::Boi lowestScoreCoords(const utils::Boi&, const std::vector<std::vector<int>>&);

	sf::RenderWindow& window;
	sf::Image image_;
	sf::Texture texture_;
	sf::Rect<int> renderRectangle_;
	sf::Sprite sprite_;
	std::vector <std::vector<sf::Color>> bitmap_;
	utils::Boi start_;
	utils::Boi end_;

	const sf::Color empty = sf::Color::White;
	const sf::Color wall = sf::Color::Black;
	const sf::Color way = sf::Color::Red;
	const sf::Color finish = sf::Color::Green;
};

