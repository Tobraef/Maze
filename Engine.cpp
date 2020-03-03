#include "Engine.h"
#include "Utility.h"
#include "GetImplementation.h"

#include <memory>

namespace _Forbidden
{

Engine::Engine(Utility& utility)
: utility_(utility)
{}

Engine::~Engine() = default;

void Engine::act()
{
	if (algorithm_)
		algorithm_();
}

std::vector<std::vector<sf::Color>> Engine::parseImageToBitmap() const
{
	std::vector<std::vector<sf::Color>> toRet;
	auto& image = utility_.image();
	const auto size = image.getSize();
	for (int i = 0; i < size.x; ++i)
	{
		toRet.emplace_back();
		for (int j = 0; j < size.y; ++j)
		{
			const auto pixel = image.getPixel(i, j);
			if (pixel.r + pixel.g + pixel.b < 384)
				toRet.back().push_back(Wall);
			else
				toRet.back().push_back(Empty);
		}
	}
}

void Engine::setAlgorithm(const AlgorithmType type)
{
	switch (type)
	{
	case AlgorithmType::Simple:
	{
		auto algo = std::shared_ptr<IAlgorithm>(GetImplementation::algorithm());
		algorithm_ = [this, algo]()
		{
			auto path = algo->findPath(this->parseImageToBitmap(), *start_, *end_);
			auto& img = utility_.image();
			for (const auto& p : path)
			{
				img.setPixel(p.x, p.y, way);
			}
			utility_.update();
			algorithm_ = nullptr;
		}; 
	}
		break;
	case AlgorithmType::Visual:
		auto vAlgo = std::shared_ptr<IVisualisedAlgorithm>(GetImplementation::visualAlgorithm());
		vAlgo->setTarget(utility_.image(), *start_, *end_);
		algorithm_ = [this, vAlgo]()
		{
			if (vAlgo->step())
			{
				algorithm_ = nullptr;
			}
			utility_.update();
		};
		break;
	}
}

void Engine::setStart(int x, int y)
{
	start_ = std::make_unique<Point>(x, y);
}

void Engine::setExit(int x, int y)
{
	end_ = std::make_unique<Point>(x, y);
}

void Engine::handleMouse(int x, int y, int button)
{
	if (button == 0)
	{
		setStart(x, y);
	}
	else
	{
		setExit(x, y);
	}
}

void Engine::handleKeyboard(char key)
{
	if (key == ' ')
	{
		if (start_ && end_)
		{
			int bx, by, tx, ty;
			bx = std::min(start_->x, end_->x);
			by = std::min(start_->y, end_->y);
			tx = std::max(start_->x, end_->x);
			ty = std::max(start_->y, end_->y);
			auto& img = utility_.image();
			for (int x = bx; x < tx; ++x)
			{
				for (int y = by; y < ty; ++y)
				{
					img.setPixel(x, y, Wall);
				}
			}
			utility_.update();
		}
	}
	else if (key == 'k')
	{
		start_ = nullptr;
		end_ = nullptr;
	}
	else if (key == 'q')
	{
		setAlgorithm(AlgorithmType::Simple);
	}
	else if (key == 'w')
	{
		setAlgorithm(AlgorithmType::Visual);
	}
}

}