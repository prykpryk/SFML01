#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Obiekt.h"
#include "PlanetaSta�a.h"
#include "ZmienneGlob.h"
#include "f_R�ne.h"
#include "f_UI.h"



void generujObiekt(std::vector<Obiekt*> *tablicaObiekt�w)
{
	mu_tObiekt�w.lock();
	auto ptr = new Obiekt(tablicaObiekt�w, 1, { 0,0 }, { 0,0 }, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiekt�w.unlock();

	if (DEBUG)
		std::cout << "Stworzono obiekt w adresie: " << ptr << "\n";
}

void narysujObiekt(RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w)
{
	Vector2f pocz�tek = window->mapPixelToCoords(Mouse::getPosition(*window));
	while (Mouse::isButtonPressed(Mouse::Left))
	{
	}
	Vector2f koniec = window->mapPixelToCoords(Mouse::getPosition(*window));
	Vector2f pr�dko�� = { (koniec.x - pocz�tek.x)*1.0f, (koniec.y - pocz�tek.y)*1.0f };
	
	
	mu_tObiekt�w.lock();
	auto ptr = new Obiekt(tablicaObiekt�w, 1, static_cast<Vector2d>(pocz�tek) / G_PIKSELI_NA_METR, static_cast<Vector2d>(pr�dko��) / G_PIKSELI_NA_METR, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiekt�w.unlock();

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << " o predkosci " << pr�dko��.x << " " << pr�dko��.y << "\n";
}

void usunPierwszyObiekt(std::vector<Obiekt*> *tablicaObiekt�w)
{
	auto it = tablicaObiekt�w->begin();
	//std::cout << *it << "\n";
	if (it != tablicaObiekt�w->end())
	{
		if (DEBUG)
			std::cout << "Usunieto obiekt w adresie: " << *it << "\n";
		delete *it;
	}
	else
		std::cout << "Nie ma nic do usuniecia.\n";
}

void usunObiektKursor(RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w)
{
	Vector2i pocz�tek = Mouse::getPosition(*window);

	Obiekt *wskazany = nullptr;

	for (Obiekt *obiekt : *tablicaObiekt�w)
	{
		Vector2i pozycjaNaEkranie = window->mapCoordsToPixel(obiekt->m_circle.getPosition());
		int odle = odl2(pocz�tek, pozycjaNaEkranie);

		if (odle <= obiekt->m_circle.getRadius())
		{
			wskazany = obiekt;
			break;
		}
	}

	delete wskazany;

}

void przesu�WidokOkna(RenderWindow &window, Vector2f a)
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
	float zmiana = 0.001f * event->mouseWheelScroll.x;		//Do poprawy, skala nie mo�e by� ujemna

	if (zmiana > 0.2)
		zmiana = 0.2;

	if (event->mouseWheelScroll.delta < 0)
		zmiana = -zmiana;

	skalujWidokOkna(*window, 1.0f + zmiana);
}
