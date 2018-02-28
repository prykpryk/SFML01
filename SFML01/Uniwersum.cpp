//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"
#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_R�ne.h"

Planeta* Uniwersum::dodajPlanet�(
	const double masa,
	const double r,
	const Vector2d pos,
	const Vector2d vel,
	const sf::Color color,
	bool zablokowana)
{
	auto ptr = new Planeta(&m_tablicaObiekt�w, masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);

	mu_tObiekt�w.lock();		//Blokada tablicy objekt�w - tworzenie planety
	m_tablicaObiekt�w.push_back(ptr);
	mu_tObiekt�w.unlock();

	return ptr;
}

Planeta* Uniwersum::dodajPlanet�(
	const double masa,
	const Vector2d pos,
	const Vector2d vel,
	const sf::Color color,
	bool zablokowana)
{
	double r = obliczPromie�(masa);
	auto ptr = new Planeta(&m_tablicaObiekt�w, masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);

	mu_tObiekt�w.lock();		//Blokada tablicy objekt�w - tworzenie planety
	m_tablicaObiekt�w.push_back(ptr);
	mu_tObiekt�w.unlock();

	return ptr;
}

double Uniwersum::obliczPromie�(double masa)
{
	return cbrt(masa);
}

void Uniwersum::usu�Planet�(Planeta* planeta)
{
	if (planeta == m_�ledzonaPlaneta)
		m_�ledzonaPlaneta = nullptr;

	//mu_tObiekt�w.lock();

	for (auto it = m_tablicaObiekt�w.begin(); it != m_tablicaObiekt�w.end(); )
	{
		if (*it == planeta)
			it = m_tablicaObiekt�w.erase(it);
		else
			it++;
	}

	//mu_tObiekt�w.unlock();

	for (auto it = tablica�lad�w.begin(); it != tablica�lad�w.end(); )
	{
		if (*it == &(planeta->m_�lad))
			it = tablica�lad�w.erase(it);
		else
			it++;
	}

	delete planeta;

}

std::vector<Planeta*>  Uniwersum::znajd�Kolizje()
{
	//sf::Clock clock;
	std::vector<Planeta*> koliduj�ce;

	for (auto a = m_tablicaObiekt�w.begin(); a != m_tablicaObiekt�w.end(); a++)
	{
		if (std::find(koliduj�ce.begin(), koliduj�ce.end(), *a) != koliduj�ce.end())
			continue;

		for (auto b = a + 1; b != m_tablicaObiekt�w.end(); b++)
		{
			if (std::find(koliduj�ce.begin(), koliduj�ce.end(), *b) != koliduj�ce.end())
				continue;

			if ((*a)->sprawd�Kolizj�(*b))
			{
				koliduj�ce.push_back(*a);
				koliduj�ce.push_back(*b);
				break;
			}
		}
	}


	//std::cout << "Liczba koliduj�cych planet: " << koliduj�ce.size() << " czas oblicze�: " << clock.restart().asMicroseconds() << "us\n";
	return koliduj�ce;
}