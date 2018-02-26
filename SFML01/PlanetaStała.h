#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Stałe.h"

class PlanetaStała : public Obiekt
{
public:
	PlanetaStała(std::vector<Obiekt*> *tablicaObiektów, double r, Vector2d pos, Vector2d vel = { 0,0 }, double masa = 10, Color color = Color::White)
		: Obiekt{ tablicaObiektów, r, pos, vel, masa, color }
	{
		m_circle.setPointCount(30);
	};
	virtual void obliczPozycję(Time &czas)
	{};
};