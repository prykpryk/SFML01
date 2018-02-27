#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Stałe.h"

class PlanetaStała : public Planeta
{
public:
	PlanetaStała(
		std::vector<Planeta*> *tablicaObiektów, 
		const double masa,
		const double r,
		const Vector2d pos,
		const Vector2d vel = { 0,0 }, 
		const sf::Color color = sf::Color::White)
		: Planeta{ tablicaObiektów,  masa, r, pos, vel, color }
	{
	};
	virtual void obliczPozycję(const double &czas)
	{};
};