#pragma once
#include "Biblioteki.h"
#include "Definicje.h"
#include "Stałe.h"

class PlanetaStała : public Obiekt
{
public:
	PlanetaStała(std::vector<Obiekt*> *tablicaObiektów, float r, sf::Vector2f pos, sf::Vector2f vel = { 0,0 }, float masa = 10, sf::Color color = sf::Color::White)
		: Obiekt{ tablicaObiektów, r, pos, vel, masa, color }
	{
	};
	virtual void obliczPozycję(sf::Time &czas)
	{};
};