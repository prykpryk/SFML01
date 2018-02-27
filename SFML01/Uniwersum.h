#pragma once

//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"
#include "PlanetaSta³a.h"

#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"
#include "f_UI.h"


class Uniwersum
{
protected:
	std::vector	<std::vector<sf::Vertex>*>	m_tablicaŒladów;
	std::vector<Planeta*>				m_tablicaObiektów;
	sf::RenderWindow						*window;
	double								m_prêdkoœæSymulacji = 1.0;
	int									m_wybranaWielkosæ = 1;


public:


	Uniwersum(sf::RenderWindow *window) : window{ window }, m_prêdkoœæSymulacji{ 1.0 }
	{};

	~Uniwersum()
	{};


	void tFizyka();

	void tRysowanie();

	void tRysowanie2();

	void tŒlady();

	Planeta * dodajPlanetê(const double masa, const double r, const Vector2d pos, const Vector2d vel, const sf::Color color, bool zablokowana = false);

	Planeta * dodajPlanetê(const double masa, const Vector2d pos, const Vector2d vel, const sf::Color color, bool zablokowana = false);

	double obliczPromieñ(double masa);

	Planeta * planetaKursor();

	void usunObiektKursor();

	void przesuñWidokOkna(sf::Vector2f a);

	void wybierzRozmiar(int i);

	void wybierzPrêdkoœæSymulacji(double i);

	void narysujObiekt();

	void zoomScroll(sf::Event & event);

	void debugujPlanetê();

	void tHandleEvents();

	void odœwie¿Kszta³y();

	void zmieñRozmiarWidoku(int w, int h);

};