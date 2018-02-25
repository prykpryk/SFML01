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
	klatka.lock();
	auto ptr = new Obiekt(tablicaObiekt�w, 1, { 0,0 }, { 0,0 }, 10, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	klatka.unlock();

	if (DEBUG)
		std::cout << "Stworzono obiekt w adresie: " << ptr << "\n";
}

void narysujObiekt(sf::RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w)
{
	sf::Vector2i pocz�tek = sf::Mouse::getPosition(*window);
	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
	}
	sf::Vector2i koniec = sf::Mouse::getPosition(*window);
	sf::Vector2f pr�dko�� = { (koniec.x - pocz�tek.x)*0.1f,(koniec.y - pocz�tek.y)*0.1f };


	klatka.lock();
	auto ptr = new Obiekt(tablicaObiekt�w, 1, { pocz�tek.x / G_PIKSELI_NA_METR, pocz�tek.y / G_PIKSELI_NA_METR }, pr�dko��, 10, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	klatka.unlock();

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << " o predkosci " << pr�dko��.x << " " << pr�dko��.y << "\n";
}

void usunObiekt(std::vector<Obiekt*> *tablicaObiekt�w)
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
