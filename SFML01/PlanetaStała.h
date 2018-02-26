#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"

class PlanetaSta³a : public Obiekt
{
public:
	PlanetaSta³a(std::vector<Obiekt*> *tablicaObiektów, double r, Vector2d pos, Vector2d vel = { 0,0 }, double masa = 10, Color color = Color::White)
		: Obiekt{ tablicaObiektów, r, pos, vel, masa, color }
	{
		m_circle.setPointCount(30);
	};
	virtual void obliczPozycjê(Time &czas)
	{};
};