#pragma once

//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"
#include "PlanetaSta�a.h"

#include "ZmienneGlob.h"
#include "f_R�ne.h"
#include "f_UI.h"


class Uniwersum
{
protected:
	std::vector	<std::vector<sf::Vertex>*>	m_tablica�lad�w;
	std::vector<Planeta*>				m_tablicaObiekt�w;
	sf::RenderWindow						*window;
	double								m_pr�dko��Symulacji = 1.0;
	int									m_wybranaWielkos� = 1;


public:


	Uniwersum(sf::RenderWindow *window) : window{ window }, m_pr�dko��Symulacji{ 1.0 }
	{};

	~Uniwersum()
	{};


	void tFizyka();

	void tRysowanie();

	void tRysowanie2();

	void t�lady();

	Planeta * dodajPlanet�(const double masa, const double r, const Vector2d pos, const Vector2d vel, const sf::Color color, bool zablokowana = false);

	Planeta * dodajPlanet�(const double masa, const Vector2d pos, const Vector2d vel, const sf::Color color, bool zablokowana = false);

	double obliczPromie�(double masa);

	Planeta * planetaKursor();

	void usunObiektKursor();

	void przesu�WidokOkna(sf::Vector2f a);

	void wybierzRozmiar(int i);

	void wybierzPr�dko��Symulacji(double i);

	void narysujObiekt();

	void zoomScroll(sf::Event & event);

	void debugujPlanet�();

	void tHandleEvents();

	void od�wie�Kszta�y();

	void zmie�RozmiarWidoku(int w, int h);

};