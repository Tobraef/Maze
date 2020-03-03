// CasualProject.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include <SFML\Graphics.hpp>

#include "Labirynth.h"

int main()
{
	const int width = 800;
	const int height = 600;
	sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
	std::string mazeDirectory = "E:\\mazes\\";
	std::cout << "Name of your maze file : ";
	std::string buffer;
	std::cin >> buffer;
	std::cout << "Start from (x, then y) : ";
	int startBuffx, startBuffy, endBuffx, endBuffy;
	std::cin >> startBuffx >> startBuffy;
	std::cout << "End at (x, then y) : ";
	std::cin >> endBuffx >> endBuffy;
	Labirynth lab(mazeDirectory + buffer + ".png", window);
	try {
		lab.solveDaMazeLikeABoss({ startBuffx, startBuffy }, { endBuffx, endBuffy });
	}
	catch (...)
	{
	}
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		lab.draw(window);
		window.display();
	}

	return 0;
}

