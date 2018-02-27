#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"

class PlanetaSta�a : public Planeta
{
public:
	PlanetaSta�a(
		std::vector<Planeta*> *tablicaObiekt�w, 
		const double masa,
		const double r,
		const Vector2d pos,
		const Vector2d vel = { 0,0 }, 
		const sf::Color color = sf::Color::White)
		: Planeta{ tablicaObiekt�w,  masa, r, pos, vel, color }
	{
	};
	virtual void obliczPozycj�(const double &czas)
	{};
};