#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Stałe.h"

class PlanetaStała : public Obiekt
{
public:
	PlanetaStała(
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
	virtual void obliczPozycję(const double &czas)
	{};
};