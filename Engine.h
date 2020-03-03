#pragma once

#include <functional>

#include "Interfaces.h"
#include "Utility.h"

namespace _Forbidden
{

	class Engine : public IEngine
	{
	public:
		Engine(Utility& utility);
		~Engine();

		void act() override;
		void setStart(int x, int y);
		void setExit(int x, int y);
		void setAlgorithm(const AlgorithmType type) override;
		void handleMouse(int x, int y, int button) override;
		void handleKeyboard(char key) override;

	private:
		std::vector<std::vector<sf::Color>> parseImageToBitmap() const;

		std::unique_ptr<Point> start_;
		std::unique_ptr<Point> end_;
		std::function<void()> algorithm_;
		Utility& utility_;
	};

}