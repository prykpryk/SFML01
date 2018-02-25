#pragma once
#include "stdafx.h"
#include "Biblioteki.h"

#include "Obiekt.h"

int rand(int min, int max);

void handleEvents(sf::RenderWindow &window, std::vector<Obiekt*> &tablicaObiektów);


int odl2(const sf::Vector2i a, const sf::Vector2i b);