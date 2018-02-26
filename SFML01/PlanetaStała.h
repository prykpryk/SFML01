#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"

class PlanetaSta³a : public Planeta
{
public:
	PlanetaSta³a(
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
	virtual void obliczPozycjê(const double &czas)
	{};
};