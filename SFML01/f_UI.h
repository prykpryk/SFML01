#pragma once

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Obiekt.h"
#include "PlanetaSta�a.h"
#include "ZmienneGlob.h"

void generujObiekt(std::vector<Obiekt*> *tablicaObiekt�w);

void narysujObiekt(RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w);

void usunPierwszyObiekt(std::vector<Obiekt*> *tablicaObiekt�w);

void usunObiektKursor(RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w);

void przesu�WidokOkna(RenderWindow &window, Vector2f a);

void skalujWidokOkna(RenderWindow &window, float a);

void zoomScroll(RenderWindow *window, Event *event);