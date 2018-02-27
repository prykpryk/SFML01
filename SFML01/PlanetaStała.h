#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"

class PlanetaSta³a : public Planeta
{
public:
	PlanetaSta³a(
		std::vector<Planeta*> *tablicaObiektów, 
		const double masa,
		const double r,
		const Vector2d pos,
		const Vector2d vel = { 0,0 }, 
		const sf::Color color = sf::Color::White)
		: Planeta{ tablicaObiektów,  masa, r, pos, vel, color }
	{
	};
	virtual void obliczPozycjê(const double &czas)
	{};
};