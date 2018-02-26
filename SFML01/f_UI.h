#pragma once

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"
#include "PlanetaSta³a.h"
#include "ZmienneGlob.h"

void handleEvents(sf::RenderWindow &window, std::vector<Planeta*> &tablicaObiektów);

void generujObiekt(std::vector<Planeta*> *tablicaObiektów);

void narysujObiekt(RenderWindow *window, std::vector<Planeta*> *tablicaObiektów);

void usunPierwszyObiekt(std::vector<Planeta*> *tablicaObiektów);

void usunObiektKursor(RenderWindow *window, std::vector<Planeta*> *tablicaObiektów);

void przesuñWidokOkna(RenderWindow &window, Vector2f a);

void skalujWidokOkna(RenderWindow &window, float a);

void zoomScroll(RenderWindow &window, Event &event);

void wybierzRozmiar(int i);

void wybierzPrêdkoœæSymulacji(double i);

void instrukcja();