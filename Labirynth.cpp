#include "Labirynth.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <queue>

using namespace utils;

Labirynth::Labirynth(const std::string& fileName, sf::RenderWindow& w)
: window(w)
{
	image_.loadFromFile(fileName);
	auto size = image_.getSize();
	renderRectangle_ = sf::Rect<int>(0, 0, size.x, size.y);
	texture_.loadFromImage(image_, renderRectangle_);
	sprite_ = sf::Sprite(texture_, renderRectangle_);
	
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
	coverEdgesOfLabirynth(getTopLeftCorner(size), getBottomRightCorner(size));
}

std::pair<int, int> Labirynth::getTopLeftCorner(const sf::Vector2<unsigned>& size) const
{
	for (int i = 0; i < size.x; ++i)
	for (int j = 0; j < size.y; ++j)
	{
		if (bitmap_[i][j] != empty)
			return std::make_pair(i,j);
	}
}

std::pair<int, int> Labirynth::getBottomRightCorner(const sf::Vector2<unsigned>& size) const
{
	for (int i = size.x-1; i > 0; --i)
	for (int j = size.y-1; j > 0; --j)
	{
		if (bitmap_[i][j] != empty)
			return std::make_pair(i, j);
	}
}

void Labirynth::coverEdgesOfLabirynth(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2)
{
	for (int i = edge1.first; i < edge2.first; ++i)
	{
		bitmap_[i][edge2.second] = wall;
		image_.setPixel(i, edge2.second, wall);
		bitmap_[i][edge1.second] = wall;
		image_.setPixel(i, edge1.second, wall);
	}
	for (int i = edge1.second; i < edge2.second; ++i)
	{
		bitmap_[edge1.first][i] = wall;
		image_.setPixel(edge1.first, i, wall);
		bitmap_[edge2.first][i] = wall;
		image_.setPixel(edge2.first, i, wall);
	}
	texture_.update(image_);
	sprite_.setTexture(texture_);
}

void Labirynth::solveDaMaze(const std::pair<int,int>& start, const std::pair<int,int>& end)
{
	start_.x = start.first; start_.y = start.second;
	end_.x = end.first; end_.y = end.second;
	Boi myBoy{ start_.x, start_.y };
	movement move = L;
	beginMovement(myBoy);
	while (!checkIfCloseEnough(myBoy))
	{
		try {
			moveDueToCurrentPosition(myBoy, move);
		}
		catch (...)
		{
			for (int i = 0; i < image_.getSize().x; ++i)
			for (int j = 0; j < image_.getSize().y; ++j)
			{
				image_.setPixel(i, j, bitmap_[i][j]);
			}
			setPicture();
			throw;
		}
	}
	markExit(myBoy);
	setPicture();
	std::cout << "found";
}

void Labirynth::draw(sf::RenderWindow& window)
{
	window.draw(sprite_);
}

void Labirynth::solveDaMazeLikeABoss(const std::pair<int, int>& start, const std::pair<int, int>& end)
{
	start_.x = start.first; 
	start_.y = start.second;
	end_.x = end.first;
	end_.y = end.second;
	std::vector<std::vector<int>> scoreBoard(bitmap_.size(), std::vector<int>(bitmap_[0].size(), 0));
	for (int i = 0; i < bitmap_.size(); ++i)
	for (int j = 0; j < bitmap_[i].size(); ++j)
	{
		if (bitmap_.at(i).at(j) == wall)
		{
			scoreBoard.at(i).at(j) = -1;
		}
	}
	try
	{
		std::cout << "Starting solving" << std::endl;
		if (exquisiteSolver({ start.first, start.second }, scoreBoard))
		{
			std::cout << "Found the end" << std::endl;
			markWayBack(scoreBoard);
		}
		else
			throw std::runtime_error{""};
	}
	catch (...) { std::cout << "Error"; setPicture(); throw; }
	std::cout << "Marked the way" << std::endl;
	setPicture();
}

bool Labirynth::exquisiteSolver(const Boi& startBoy, std::vector<std::vector<int>>& scoreBoard)
{
	std::queue<std::pair<Boi, int>> awaitingBoys;
	awaitingBoys.emplace(std::make_pair(startBoy, 1));
	scoreBoard.at(startBoy.x).at(startBoy.y) = 1;
	while (!awaitingBoys.empty())
	{
		auto myScoreBoy = awaitingBoys.front();
		auto myBoy = myScoreBoy.first;
		auto score = myScoreBoy.second;
		bitmap_.at(myBoy.x).at(myBoy.y) = way;
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

bool Labirynth::checkIfFoundExit(const Boi& boy)
{
	return (boy.x == end_.x && boy.y == end_.y);
}

Boi Labirynth::lowestScoreCoords(const Boi& b, const std::vector<std::vector<int>>& scores)
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

void Labirynth::markWayBack(const std::vector<std::vector<int>>& scoreBoard)
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

void Labirynth::moveDueToCurrentPosition(Boi& boy, movement& move)
{
	auto left = bitmap_.at(boy.x-1).at(boy.y);
	auto right = bitmap_.at(boy.x+1).at(boy.y);
	auto top = bitmap_.at(boy.x).at(boy.y - 1);
	auto bottom = bitmap_.at(boy.x).at(boy.y - 1);

	switch (move)
	{
	case G:
		left = bitmap_.at(boy.x-1).at(boy.y);
		right = bitmap_.at(boy.x+1).at(boy.y);
		top = bitmap_.at(boy.x).at(boy.y - 1);
		if (left == right)
		{
			if (bitmap_.at(boy.x+1).at(boy.y + 1) == wall)
				move = P;
			else
				move = L;
		}
		else if (left != empty)
		{
			if (top != empty)
				move = P;
			else
				move = G;
		}
		else
		{
			if (top != empty)
				move = L;
			else
				move = G;
		}
		break;
	case D:
		left = bitmap_.at(boy.x-1).at(boy.y);
		right = bitmap_.at(boy.x+1).at(boy.y);
		bottom = bitmap_.at(boy.x).at(boy.y + 1);
		if (left == right)
		{
			if (bitmap_.at(boy.x-1).at(boy.y - 1) == wall)
				move = L;
			else
				move = P;
		}
		else if (left != empty)
		{
			if (bottom != empty)
				move = P;
			else
				move = D;
		}
		else
		{
			if (bottom != empty)
				move = L;
			else
				move = D;
		}
		break;
	case P:
		top = bitmap_.at(boy.x).at(boy.y - 1);
		bottom = bitmap_.at(boy.x).at(boy.y + 1);
		right = bitmap_.at(boy.x+1).at(boy.y);
		if (top == bottom)
		{
			if (bitmap_.at(boy.x-1).at(boy.y - 1) == wall)
				move = G;
			else
				move = D;
		}
		else if (top != empty)
		{
			if (right != empty)
				move = D;
			else
				move = P;
		}
		else
		{
			if (right != empty)
				move = G;
			else
				move = P;
		}
		break;
	case L:
		top = bitmap_.at(boy.x).at(boy.y - 1);
		bottom = bitmap_.at(boy.x).at(boy.y + 1);
		left = bitmap_.at(boy.x-1).at(boy.y);
		if (top == bottom)
		{
			if (bitmap_.at(boy.x+1).at(boy.y + 1) == wall)
				move = D;
			else
				move = G;
		}
		else if (top == wall)
		{
			if (left == wall)
				move = D;
			else
				move = L;
		}
		else
		{
			if (left == wall)
				move = G;
			else
				move = L;
		}
		break;
	}

	moveMap.at(move)(boy);
	bitmap_.at(boy.x).at(boy.y) = way;
}

bool Labirynth::checkIfCloseEnough(const Boi boy) const
{
	return (::abs(boy.x - end_.x) <= 5 && ::abs(boy.y - end_.y) <= 5);
}

void Labirynth::beginMovement(Boi& boy)
{
	for (; bitmap_.at(boy.x).at(boy.y - 1) != wall;)
	{
		moveMap.at(G)(boy);
		bitmap_.at(boy.x).at(boy.y) = way;
	}
}

void Labirynth::setPicture()
{
	for (int i = 0; i < image_.getSize().x; ++i)
	for (int j = 0; j < image_.getSize().y; ++j)
	{
		image_.setPixel(i, j, bitmap_[i][j]);
	}
	texture_.update(image_);
	sprite_.setTexture(texture_);
}

void Labirynth::markExit(const Boi& b)
{
	for (int i = b.x - 5; i < b.x + 5; ++i)
		image_.setPixel(i, b.y, finish);
	for (int i = b.y - 5; i < b.y + 5; ++i)
		image_.setPixel(b.x, i, finish);
}