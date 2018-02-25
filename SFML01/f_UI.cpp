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
	klatka.lock();
	auto ptr = new Obiekt(tablicaObiektów, 1, { 0,0 }, { 0,0 }, 10, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	klatka.unlock();

	if (DEBUG)
		std::cout << "Stworzono obiekt w adresie: " << ptr << "\n";
}

void narysujObiekt(sf::RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów)
{
	sf::Vector2i pocz¹tek = sf::Mouse::getPosition(*window);
	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
	}
	sf::Vector2i koniec = sf::Mouse::getPosition(*window);
	sf::Vector2f prêdkoœæ = { (koniec.x - pocz¹tek.x)*0.1f,(koniec.y - pocz¹tek.y)*0.1f };


	klatka.lock();
	auto ptr = new Obiekt(tablicaObiektów, 1, { pocz¹tek.x / G_PIKSELI_NA_METR, pocz¹tek.y / G_PIKSELI_NA_METR }, prêdkoœæ, 10, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	klatka.unlock();

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

void usunObiektKursor(sf::RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów)
{
	sf::Vector2i pocz¹tek = sf::Mouse::getPosition(*window);

	Obiekt *wskazany = nullptr;

	for (Obiekt *obiekt : *tablicaObiektów)
	{
		sf::Vector2i pozycjaNaEkranie = window->mapCoordsToPixel(obiekt->m_circle.getPosition());
		int odle = odl2(pocz¹tek, pozycjaNaEkranie);

		if (odle <= obiekt->m_circle.getRadius())
		{
			wskazany = obiekt;
			break;
		}
	}

	delete wskazany;

}
