#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Stałe.h"

#include "Obiekt.h"
#include "PlanetaStała.h"
#include "ZmienneGlob.h"
#include "f_Różne.h"
#include "f_UI.h"



void generujObiekt(std::vector<Obiekt*> *tablicaObiektów)
{
	mu_tObiektów.lock();
	auto ptr = new Obiekt(tablicaObiektów, 1, { 0,0 }, { 0,0 }, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiektów.unlock();

	if (DEBUG)
		std::cout << "Stworzono obiekt w adresie: " << ptr << "\n";
}

void narysujObiekt(RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów)
{
	Vector2i początek = Mouse::getPosition(*window);
	while (Mouse::isButtonPressed(Mouse::Left))
	{
	}
	Vector2i koniec = Mouse::getPosition(*window);
	Vector2d prędkość = { (koniec.x - początek.x)*0.1, (koniec.y - początek.y)*0.1 };


	mu_tObiektów.lock();
	auto ptr = new Obiekt(tablicaObiektów, 1, { początek.x / G_PIKSELI_NA_METR, początek.y / G_PIKSELI_NA_METR }, prędkość, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiektów.unlock();

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << " o predkosci " << prędkość.x << " " << prędkość.y << "\n";
}

void usunPierwszyObiekt(std::vector<Obiekt*> *tablicaObiektów)
{
	auto it = tablicaObiektów->begin();
	//std::cout << *it << "\n";
	if (it != tablicaObiektów->end())
	{
		if (DEBUG)
			std::cout << "Usunieto obiekt w adresie: " << *it << "\n";
		delete *it;
	}
	else
		std::cout << "Nie ma nic do usuniecia.\n";
}

void usunObiektKursor(RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów)
{
	Vector2i początek = Mouse::getPosition(*window);

	Obiekt *wskazany = nullptr;

	for (Obiekt *obiekt : *tablicaObiektów)
	{
		Vector2i pozycjaNaEkranie = window->mapCoordsToPixel(obiekt->m_circle.getPosition());
		int odle = odl2(początek, pozycjaNaEkranie);

		if (odle <= obiekt->m_circle.getRadius())
		{
			wskazany = obiekt;
			break;
		}
	}

	delete wskazany;

}

void przesuńWidokOkna(RenderWindow &window, Vector2f a)
{
	View view = window.getView();
	view.move(a);
	window.setView(view);
}

void skalujWidokOkna(RenderWindow &window, float a)
{
	View view = window.getView();
	view.zoom(a);
	window.setView(view);
}