#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"

class PlanetaSta³a : public Obiekt
{
public:
	PlanetaSta³a(std::vector<Obiekt*> *tablicaObiektów, float r, sf::Vector2f pos, sf::Vector2f vel = { 0,0 }, float masa = 10, sf::Color color = sf::Color::White)
		: Obiekt{ tablicaObiektów, r, pos, vel, masa, color }
	{
	};
	virtual void obliczPozycjê(sf::Time &czas)
	{};
};