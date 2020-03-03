#include "VisualAlgorithm.h"

#include <iostream>

namespace _Forbidden
{
	using namespace utils;
	void VisualAlgorithm::setTarget(sf::Image& bitmap, const Point start, const Point end)
	{
		image_ = bitmap;
		const auto size = image_.getSize();
		auto pixArray = image_.getPixelsPtr();
		for (int i = 0; i < size.x; ++i)
		{
			bitmap_.push_back(std::vector<sf::Color>());
			for (int j = 0; j < size.y; ++j)
			{
				sf::Color color = empty;
				if ((int)pixArray[(i + j*size.x) * 4] +
					(int)pixArray[(i + j*size.x) * 4 + 1] +
					(int)pixArray[(i + j*size.x) * 4 + 2] < 384)
					color = wall;
				bitmap_.back().push_back(color);
			}
		}
		start_ = { start.x, start.y };
		end_ = { end.x, end.y };
	}


	bool VisualAlgorithm::step()
	{
		if (paintQueue_.empty())
		{
			if (bitmap_.empty())
				return true;
			solve();
		}
		else
		{
			updateImage();
		}
		return false;
	}

	void VisualAlgorithm::updateImage()
	{
		for (int i = 0; i < std::min(std::size_t(10), paintQueue_.size()); ++i)
		{
			auto& f = paintQueue_.front();
			image_.setPixel(f.first.x, f.first.y, f.second);
			paintQueue_.pop();
		}
	}

	void VisualAlgorithm::solve()
	{

	}

	void VisualAlgorithm::solveDaMazeLikeABoss()
	{
		std::vector<std::vector<int>> scoreBoard(bitmap_.size(), std::vector<int>(bitmap_[0].size(), 0));
		for (int i = 0; i < bitmap_.size(); ++i)
		for (int j = 0; j < bitmap_[i].size(); ++j)
		{
			if (bitmap_.at(i).at(j) == wall)
			{
				scoreBoard.at(i).at(j) = -1;
			}
		}
		std::cout << "Starting solving" << std::endl;
		if (exquisiteSolver(scoreBoard))
		{
			std::cout << "Found the end" << std::endl;
			markWayBack(scoreBoard);
		}
		std::cout << "Marked the way" << std::endl;
	}

	bool VisualAlgorithm::exquisiteSolver(std::vector<std::vector<int>>& scoreBoard)
	{
		Boi startBoy{ start_.x, start_.y };
		std::queue<std::pair<Boi, int>> awaitingBoys;
		awaitingBoys.emplace(std::make_pair(startBoy, 1));
		scoreBoard.at(startBoy.x).at(startBoy.y) = 1;
		while (!awaitingBoys.empty())
		{
			auto myScoreBoy = awaitingBoys.front();
			paintQueue_.push(std::make_pair(Point{ myScoreBoy.first.x, myScoreBoy.first.y }, way));
			auto myBoy = myScoreBoy.first;
			auto score = myScoreBoy.second;
			if (checkIfFoundExit(myBoy))
				return true;
			switch (scoreBoard.at(myBoy.x + 1).at(myBoy.y))
			{
			case 0:
				awaitingBoys.emplace(std::make_pair(Boi(myBoy.x + 1, myBoy.y), score + 1));
				scoreBoard.at(myBoy.x + 1).at(myBoy.y) = score + 1;
				break;
			case -1:
				break;
			default:
				if (scoreBoard.at(myBoy.x + 1).at(myBoy.y) > score + 1)
				{
					scoreBoard.at(myBoy.x + 1).at(myBoy.y) = score + 1;
				}
				break;
			}

			switch (scoreBoard.at(myBoy.x - 1).at(myBoy.y))
			{
			case 0:
				awaitingBoys.emplace(std::make_pair(Boi(myBoy.x - 1, myBoy.y), score + 1));
				scoreBoard.at(myBoy.x - 1).at(myBoy.y) = score + 1;
				break;
			case -1:
				break;
			default:
				if (scoreBoard.at(myBoy.x - 1).at(myBoy.y) > score + 1)
				{
					scoreBoard.at(myBoy.x - 1).at(myBoy.y) = score + 1;
				}
				break;
			}

			switch (scoreBoard.at(myBoy.x).at(myBoy.y + 1))
			{
			case 0:
				awaitingBoys.emplace(std::make_pair(Boi(myBoy.x, myBoy.y + 1), score + 1));
				scoreBoard.at(myBoy.x).at(myBoy.y + 1) = score + 1;
				break;
			case -1:
				break;
			default:
				if (scoreBoard.at(myBoy.x).at(myBoy.y + 1) > score + 1)
				{
					scoreBoard.at(myBoy.x).at(myBoy.y + 1) = score + 1;
				}
				break;
			}

			switch (scoreBoard.at(myBoy.x).at(myBoy.y - 1))
			{
			case 0:
				awaitingBoys.emplace(std::make_pair(Boi(myBoy.x, myBoy.y - 1), score + 1));
				scoreBoard.at(myBoy.x).at(myBoy.y - 1) = score + 1;
				break;
			case -1:
				break;
			default:
				if (scoreBoard.at(myBoy.x).at(myBoy.y - 1) > score + 1)
				{
					scoreBoard.at(myBoy.x).at(myBoy.y - 1) = score + 1;
				}
				break;
			}
			awaitingBoys.pop();
		}
		return false;
	}

	bool VisualAlgorithm::checkIfFoundExit(const Boi& boy)
	{
		return (boy.x == end_.x && boy.y == end_.y);
	}

	void VisualAlgorithm::markWayBack(const std::vector<std::vector<int>>& scoreBoard)
	{
		Boi boy{ end_.x, end_.y };
		while (true)
		{
			if (boy.x == start_.x && boy.y == start_.y)
				break;
			auto nextMove = lowestScoreCoords(boy, scoreBoard);
			boy.x = nextMove.x;
			boy.y = nextMove.y;
			bitmap_.at(boy.x).at(boy.y) = finish;
		}
	}

	Boi VisualAlgorithm::lowestScoreCoords(const Boi& b, const std::vector<std::vector<int>>& scores)
	{
		std::map<Boi, int> scoreCoords;
		if (scores.at(b.x + 1).at(b.y) > 0)
			scoreCoords[{ b.x + 1, b.y}] = scores.at(b.x + 1).at(b.y);
		if (scores.at(b.x - 1).at(b.y) > 0)
			scoreCoords[{ b.x - 1, b.y}] = scores.at(b.x - 1).at(b.y);
		if (scores.at(b.x).at(b.y + 1) > 0)
			scoreCoords[{ b.x, b.y + 1}] = scores.at(b.x).at(b.y + 1);
		if (scores.at(b.x).at(b.y - 1) > 0)
			scoreCoords[{ b.x, b.y - 1}] = scores.at(b.x).at(b.y - 1);
		std::pair<Boi, int> lowestPair = *scoreCoords.begin();
		std::for_each(std::next(scoreCoords.begin()), scoreCoords.end(), [&lowestPair](const std::pair<Boi, int>& c)
		{
			if (c.second < lowestPair.second)
				lowestPair = c;
		});
		return lowestPair.first;
	}
}
