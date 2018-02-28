//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"
#include "PlanetaSta�a.h"
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

	mu_tObiekt�w.lock();
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

	mu_tObiekt�w.lock();
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

	for (auto it = m_tablicaObiekt�w.begin(); it != m_tablicaObiekt�w.end(); )
	{
		if (*it == planeta)
			it = m_tablicaObiekt�w.erase(it);
		else
			it++;
	}

	for (auto it = tablica�lad�w.begin(); it != tablica�lad�w.end(); )
	{
		if (*it == &(planeta->m_�lad))
			it = tablica�lad�w.erase(it);
		else
			it++;
	}

	delete planeta;
}
