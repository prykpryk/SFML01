#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"

class PlanetaSta�a : public Obiekt
{
public:
	PlanetaSta�a(std::vector<Obiekt*> *tablicaObiekt�w, float r, sf::Vector2f pos, sf::Vector2f vel = { 0,0 }, float masa = 10, sf::Color color = sf::Color::White)
		: Obiekt{ tablicaObiekt�w, r, pos, vel, masa, color }
	{
	};
	virtual void obliczPozycj�(sf::Time &czas)
	{};
};