#pragma once

//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"

#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"
#include "f_UI.h"


class Uniwersum
{
protected:
	//std::vector	<std::vector<sf::Vertex>>	m_tablicaŒladów;
	std::vector<Planeta>				m_tablicaObiektów;		//TODO A jakby tak po prostu wszystkie planety daæ do wektora?
	sf::RenderWindow						*window;
	double								m_prêdkoœæSymulacji = 1.0;
	int									m_wybranaWielkosæ = 1;
	Planeta								*m_œledzonaPlaneta = nullptr;

	std::mutex	mu_tObiektów;

public:


	Uniwersum(sf::RenderWindow *window) : window{ window }, m_prêdkoœæSymulacji{ 1.0 }
	{
	};

	~Uniwersum()
	{};


	void little_sleep(std::chrono::microseconds us);

	void tFizyka();

	void tRysowanie();

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

	void handleEvents();

	void odœwie¿Kszta³y();

	void zmieñRozmiarWidoku(int w, int h);

	void œledŸPlanetê();

	void ustawŒledzon¹Planetê(Planeta * planeta);

	void usuñPlanetê(Planeta & planeta);

	std::vector<Planeta>::iterator znajdŸIterator(const Planeta & planeta);

	Vector2d obliczSi³yGrawitacji(const Planeta & planeta);

	Vector2d obliczSi³yNapêdu(const Planeta & a);

	void obliczPozycjê(Planeta & planeta, const double & czas);

	void ustawŒledzon¹PlanetêKursor();

	bool prze³¹czPauzê();

	void kolizje();
};