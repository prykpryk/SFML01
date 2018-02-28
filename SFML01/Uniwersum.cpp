//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"
#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"

Planeta* Uniwersum::dodajPlanetê(
	const double masa,
	const double r,
	const Vector2d pos,
	const Vector2d vel,
	const sf::Color color,
	bool zablokowana)
{
	auto ptr = new Planeta(&m_tablicaObiektów, masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);

	mu_tObiektów.lock();		//Blokada tablicy objektów - tworzenie planety
	m_tablicaObiektów.push_back(ptr);
	mu_tObiektów.unlock();

	return ptr;
}

Planeta* Uniwersum::dodajPlanetê(
	const double masa,
	const Vector2d pos,
	const Vector2d vel,
	const sf::Color color,
	bool zablokowana)
{
	double r = obliczPromieñ(masa);
	auto ptr = new Planeta(&m_tablicaObiektów, masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);

	mu_tObiektów.lock();		//Blokada tablicy objektów - tworzenie planety
	m_tablicaObiektów.push_back(ptr);
	mu_tObiektów.unlock();

	return ptr;
}

double Uniwersum::obliczPromieñ(double masa)
{
	return cbrt(masa);
}

void Uniwersum::usuñPlanetê(Planeta* planeta)
{
	if (planeta == m_œledzonaPlaneta)
		m_œledzonaPlaneta = nullptr;

	//mu_tObiektów.lock();

	for (auto it = m_tablicaObiektów.begin(); it != m_tablicaObiektów.end(); )
	{
		if (*it == planeta)
			it = m_tablicaObiektów.erase(it);
		else
			it++;
	}

	//mu_tObiektów.unlock();

	for (auto it = tablicaŒladów.begin(); it != tablicaŒladów.end(); )
	{
		if (*it == &(planeta->m_œlad))
			it = tablicaŒladów.erase(it);
		else
			it++;
	}

	delete planeta;

}

std::vector<Planeta*>  Uniwersum::znajdŸKolizje()
{
	//sf::Clock clock;
	std::vector<Planeta*> koliduj¹ce;

	for (auto a = m_tablicaObiektów.begin(); a != m_tablicaObiektów.end(); a++)
	{
		if (std::find(koliduj¹ce.begin(), koliduj¹ce.end(), *a) != koliduj¹ce.end())
			continue;

		for (auto b = a + 1; b != m_tablicaObiektów.end(); b++)
		{
			if (std::find(koliduj¹ce.begin(), koliduj¹ce.end(), *b) != koliduj¹ce.end())
				continue;

			if ((*a)->sprawdŸKolizjê(*b))
			{
				koliduj¹ce.push_back(*a);
				koliduj¹ce.push_back(*b);
				break;
			}
		}
	}


	//std::cout << "Liczba koliduj¹cych planet: " << koliduj¹ce.size() << " czas obliczeñ: " << clock.restart().asMicroseconds() << "us\n";
	return koliduj¹ce;
}