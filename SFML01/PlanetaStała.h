#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"

class PlanetaSta³a : public Obiekt
{
public:
	PlanetaSta³a(
		std::vector<Obiekt*> *tablicaObiektów,
		const double r,
		const Vector2d pos,
		const Vector2d vel = { 0,0 }, 
		const double masa = 10, 
		const Color color = Color::White)
		: Obiekt{ tablicaObiektów, r, pos, vel, masa, color }
	{
		m_circle.setPointCount(30);
	};
	virtual void obliczPozycjê(const double &czas)
	{};
};