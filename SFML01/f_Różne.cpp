//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"

#include "ZmienneGlob.h"

#include "f_R�ne.h"
#include "f_UI.h"

int rand(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 mersenne{ rd() };
	static const double fraction = 1.0 / (static_cast<double>(mersenne.max()) + 1.0);
	return min + static_cast<int>((max - min + 1) * (mersenne() * fraction));
}

int odl2(sf::Vector2i a, sf::Vector2i b)
{
	sf::Vector2i c = b - a;
	return static_cast<int>(sqrt(c.x*c.x + c.y*c.y));
}

float odl2(sf::Vector2f a, sf::Vector2f b)
{
	sf::Vector2f c = b - a;
	return sqrt(c.x*c.x + c.y*c.y);
}