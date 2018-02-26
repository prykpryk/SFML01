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
	Vector2i pocz�tek = Mouse::getPosition(*window);
	while (Mouse::isButtonPressed(Mouse::Left))
	{
	}
	Vector2i koniec = Mouse::getPosition(*window);
	Vector2d pr�dko�� = { (koniec.x - pocz�tek.x)*0.1, (koniec.y - pocz�tek.y)*0.1 };


	mu_tObiekt�w.lock();
	auto ptr = new Obiekt(tablicaObiekt�w, 1, { pocz�tek.x / G_PIKSELI_NA_METR, pocz�tek.y / G_PIKSELI_NA_METR }, pr�dko��, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
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
	view.move(a);
	window.setView(view);
}

void skalujWidokOkna(RenderWindow &window, float a)
{
	View view = window.getView();
	view.zoom(a);
	window.setView(view);
}