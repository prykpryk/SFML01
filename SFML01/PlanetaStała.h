#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"

class PlanetaSta�a : public Planeta
{
public:
	PlanetaSta�a(
		std::vector<Planeta*> *tablicaObiekt�w,
		const double r,
		const Vector2d pos,
		const Vector2d vel = { 0,0 }, 
		const double masa = 10, 
		const Color color = Color::White)
		: Planeta{ tablicaObiekt�w, r, pos, vel, masa, color }
	{
		setPointCount(30);
	};
	virtual void obliczPozycj�(const double &czas)
	{};
};