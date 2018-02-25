#pragma once

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Obiekt.h"
#include "PlanetaSta�a.h"
#include "ZmienneGlob.h"

void generujObiekt(std::vector<Obiekt*> *tablicaObiekt�w);

void narysujObiekt(sf::RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w);

void usunPierwszyObiekt(std::vector<Obiekt*> *tablicaObiekt�w);

void usunObiektKursor(sf::RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w);