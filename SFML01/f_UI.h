#pragma once

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Obiekt.h"
#include "PlanetaSta³a.h"
#include "ZmienneGlob.h"

void generujObiekt(std::vector<Obiekt*> *tablicaObiektów);

void narysujObiekt(sf::RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów);

void usunObiekt(std::vector<Obiekt*> *tablicaObiektów);