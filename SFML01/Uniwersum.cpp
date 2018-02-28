//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Stałe.h"

#include "Planeta.h"
#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_Różne.h"

Planeta* Uniwersum::dodajPlanetę(
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

Planeta* Uniwersum::dodajPlanetę(
	const double masa,
	const Vector2d pos,
	const Vector2d vel,
	const sf::Color color,
	bool zablokowana)
{
	double r = obliczPromień(masa);
	auto ptr = new Planeta(&m_tablicaObiektów, masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);

	mu_tObiektów.lock();
	m_tablicaObiektów.push_back(ptr);
	mu_tObiektów.unlock();

	return ptr;
}

double Uniwersum::obliczPromień(double masa)
{
	return cbrt(masa);
}

void Uniwersum::usuńPlanetę(Planeta* planeta)
{
	if (planeta == m_śledzonaPlaneta)
		m_śledzonaPlaneta = nullptr;

	for (auto it = m_tablicaObiektów.begin(); it != m_tablicaObiektów.end(); )
	{
		if (*it == planeta)
			it = m_tablicaObiektów.erase(it);
		else
			it++;
	}

	for (auto it = tablicaŚladów.begin(); it != tablicaŚladów.end(); )
	{
		if (*it == &(planeta->m_ślad))
			it = tablicaŚladów.erase(it);
		else
			it++;
	}

	delete planeta;
}
