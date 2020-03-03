#pragma once
#include "Interfaces.h"
#include <functional>
#include <queue>

namespace _Forbidden
{
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

	class VisualAlgorithm : public IVisualisedAlgorithm
	{
	public:
		~VisualAlgorithm() = default;

		void setTarget(sf::Image& bitmap, const Point start, const Point end) override;
		bool step() override;
		void solve();
		void updateImage();

	private:
		sf::Image& image_;
		std::vector <std::vector<sf::Color>> bitmap_;
		utils::Boi start_;
		utils::Boi end_;
		std::queue<std::pair<Point, sf::Color>> paintQueue_;

		void solveDaMazeLikeABoss();
		bool exquisiteSolver(std::vector<std::vector<int>>& scoreBoard);
		bool checkIfFoundExit(const Boi& boy);
		void markWayBack(const std::vector<std::vector<int>>& scoreBoard);
		utils::Boi lowestScoreCoords(const Boi& b, const std::vector<std::vector<int>>& scores);

		const sf::Color empty = sf::Color::White;
		const sf::Color wall = sf::Color::Black;
		const sf::Color way = sf::Color::Red;
		const sf::Color finish = sf::Color::Green;
	};
}



