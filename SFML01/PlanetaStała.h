#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"

class PlanetaSta�a : public Obiekt
{
public:
	PlanetaSta�a(std::vector<Obiekt*> *tablicaObiekt�w, double r, Vector2d pos, Vector2d vel = { 0,0 }, double masa = 10, Color color = Color::White)
		: Obiekt{ tablicaObiekt�w, r, pos, vel, masa, color }
	{
		m_circle.setPointCount(30);
	};
	virtual void obliczPozycj�(Time &czas)
	{};
};