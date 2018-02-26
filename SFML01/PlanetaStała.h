#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Stałe.h"

class PlanetaStała : public Planeta
{
public:
	PlanetaStała(
		std::vector<Planeta*> *tablicaObiektów,
		const double r,
		const Vector2d pos,
		const Vector2d vel = { 0,0 }, 
		const double masa = 10, 
		const Color color = Color::White)
		: Planeta{ tablicaObiektów, r, pos, vel, masa, color }
	{
		setPointCount(30);
	};
	virtual void obliczPozycję(const double &czas)
	{};
};