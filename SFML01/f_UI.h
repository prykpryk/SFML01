#pragma once

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Stałe.h"

#include "Obiekt.h"
#include "PlanetaStała.h"
#include "ZmienneGlob.h"

void generujObiekt(std::vector<Obiekt*> *tablicaObiektów);

void narysujObiekt(sf::RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów);

void usunObiekt(std::vector<Obiekt*> *tablicaObiektów);