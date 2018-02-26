#pragma once

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"
#include "PlanetaSta�a.h"
#include "ZmienneGlob.h"

void handleEvents(sf::RenderWindow &window, std::vector<Planeta*> &tablicaObiekt�w);

void generujObiekt(std::vector<Planeta*> *tablicaObiekt�w);

void narysujObiekt(RenderWindow *window, std::vector<Planeta*> *tablicaObiekt�w);

void usunPierwszyObiekt(std::vector<Planeta*> *tablicaObiekt�w);

void usunObiektKursor(RenderWindow *window, std::vector<Planeta*> *tablicaObiekt�w);

void przesu�WidokOkna(RenderWindow &window, Vector2f a);

void skalujWidokOkna(RenderWindow &window, float a);

void zoomScroll(RenderWindow &window, Event &event);

void wybierzRozmiar(int i);

void wybierzPr�dko��Symulacji(double i);

void instrukcja();