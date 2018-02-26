#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Obiekt.h"
#include "PlanetaSta³a.h"
#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"
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
	Vector2f pocz¹tek = window->mapPixelToCoords(Mouse::getPosition(*window));
	while (Mouse::isButtonPressed(Mouse::Left))
	{
	}
	Vector2f koniec = window->mapPixelToCoords(Mouse::getPosition(*window));
	Vector2f prêdkoœæ = { (koniec.x - pocz¹tek.x)*1.0f, (koniec.y - pocz¹tek.y)*1.0f };
	
	
	mu_tObiektów.lock();
	auto ptr = new Obiekt(tablicaObiektów, 1, static_cast<Vector2d>(pocz¹tek) / G_PIKSELI_NA_METR, static_cast<Vector2d>(prêdkoœæ) / G_PIKSELI_NA_METR, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiektów.unlock();

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << " o predkosci " << prêdkoœæ.x << " " << prêdkoœæ.y << "\n";
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
	Vector2i pocz¹tek = Mouse::getPosition(*window);

	Obiekt *wskazany = nullptr;

	for (Obiekt *obiekt : *tablicaObiektów)
	{
		Vector2i pozycjaNaEkranie = window->mapCoordsToPixel(obiekt->m_circle.getPosition());
		int odle = odl2(pocz¹tek, pozycjaNaEkranie);

		if (odle <= obiekt->m_circle.getRadius())
		{
			wskazany = obiekt;
			break;
		}
	}

	delete wskazany;

}

void przesuñWidokOkna(RenderWindow &window, Vector2f a)
{
	View view = window.getView();
	view.move(a * view.getSize().x / 160.0f);
	window.setView(view);
}

void skalujWidokOkna(RenderWindow &window, float a)
{
	View view = window.getView();
	view.zoom(a);
	window.setView(view);
}

void zoomScroll(RenderWindow *window, Event *event)
{
	float zmiana = 0.001f * event->mouseWheelScroll.x;		//Do poprawy, skala nie mo¿e byæ ujemna

	if (zmiana > 0.2)
		zmiana = 0.2;

	if (event->mouseWheelScroll.delta < 0)
		zmiana = -zmiana;

	skalujWidokOkna(*window, 1.0f + zmiana);
}
