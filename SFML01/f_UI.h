#pragma once

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Obiekt.h"
#include "PlanetaSta³a.h"
#include "ZmienneGlob.h"

void generujObiekt(std::vector<Obiekt*> *tablicaObiektów);

void narysujObiekt(RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów);

void usunPierwszyObiekt(std::vector<Obiekt*> *tablicaObiektów);

void usunObiektKursor(RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów);

void przesuñWidokOkna(RenderWindow &window, Vector2f a);

void skalujWidokOkna(RenderWindow &window, float a);

void zoomScroll(RenderWindow *window, Event *event);