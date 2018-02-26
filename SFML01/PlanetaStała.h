#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"

class PlanetaSta�a : public Obiekt
{
public:
	PlanetaSta�a(
		std::vector<Obiekt*> *tablicaObiekt�w,
		const double r,
		const Vector2d pos,
		const Vector2d vel = { 0,0 }, 
		const double masa = 10, 
		const Color color = Color::White)
		: Obiekt{ tablicaObiekt�w, r, pos, vel, masa, color }
	{
		m_circle.setPointCount(30);
	};
	virtual void obliczPozycj�(const double &czas)
	{};
};