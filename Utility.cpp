#include "Utility.h"


Utility::Utility(const std::string& fileName, sf::RenderWindow& window)
: window_(window)
{
	image_.loadFromFile(fileName);
	auto size = image_.getSize();
	renderRectangle_ = sf::Rect<int>(0, 0, size.x, size.y);
	texture_.loadFromImage(image_, renderRectangle_);
	sprite_ = sf::Sprite(texture_, renderRectangle_);

	auto pixArray = image_.getPixelsPtr();
	for (int i = 0; i < size.x; ++i)
	{
		for (int j = 0; j < size.y; ++j)
		{
			if ((int)pixArray[(i + j*size.x) * 4] +
				(int)pixArray[(i + j*size.x) * 4 + 1] +
				(int)pixArray[(i + j*size.x) * 4 + 2] < 384)
				image_.setPixel(i, j, wall);
			else
				image_.setPixel(i, j, empty);
		}
	}
	for (int i = 0; i < size.x; ++i) { image_.setPixel(i, 0, wall); }
	for (int j = 0; j < size.y; ++j) { image_.setPixel(0, j, wall); }
	for (int i = 0; i < size.x; ++i) { image_.setPixel(i, size.y - 1, wall); }
	for (int j = 0; j < size.y; ++j) { image_.setPixel(size.x - 1, j, wall); }
}


Utility::~Utility() = default;

sf::Image& Utility::image()
{
	return image_;
}

void Utility::update()
{
	texture_.update(image_);
	sprite_.setTexture(texture_);
}

void Utility::draw()
{
	window_.draw(sprite_);
}