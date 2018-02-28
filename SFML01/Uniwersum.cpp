//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"
#include "PlanetaSta³a.h"
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

	mu_tObiektów.lock();
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

	mu_tObiektów.lock();
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

	for (auto it = m_tablicaObiektów.begin(); it != m_tablicaObiektów.end(); )
	{
		if (*it == planeta)
			it = m_tablicaObiektów.erase(it);
		else
			it++;
	}

	for (auto it = tablicaŒladów.begin(); it != tablicaŒladów.end(); )
	{
		if (*it == &(planeta->m_œlad))
			it = tablicaŒladów.erase(it);
		else
			it++;
	}

	delete planeta;
}
