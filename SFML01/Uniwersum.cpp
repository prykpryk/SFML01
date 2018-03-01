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
	mu_tObiektów.lock();		//Blokada tablicy objektów - tworzenie planety
	m_tablicaObiektów.emplace_back(masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);
	mu_tObiektów.unlock();

	return &(*(m_tablicaObiektów.end() - 1));;
}

Planeta* Uniwersum::dodajPlanetê(
	const double masa,
	const Vector2d pos,
	const Vector2d vel,
	const sf::Color color,
	bool zablokowana)
{
	double r = obliczPromieñ(masa);

	mu_tObiektów.lock();		//Blokada tablicy objektów - tworzenie planety
	m_tablicaObiektów.emplace_back(masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);
	mu_tObiektów.unlock();

	return &(*(m_tablicaObiektów.end()-1));
}

double Uniwersum::obliczPromieñ(double masa)
{
	return cbrt(masa);
}

void Uniwersum::usuñPlanetê(Planeta & planeta)
{
	if (&planeta == m_œledzonaPlaneta)
		m_œledzonaPlaneta = nullptr;

	/*for (auto it = tablicaŒladów.begin(); it != tablicaŒladów.end(); )
	{
		if (*it == &(planeta->m_œlad))
			it = tablicaŒladów.erase(it);
		else
			it++;
	}
	*/

	mu_tObiektów.lock();		//Blokada tablicy objektów - usuwanie planety

	auto it = znajdŸIterator(planeta);

	if (it != m_tablicaObiektów.end())
		m_tablicaObiektów.erase(it);

	mu_tObiektów.unlock();

}

std::vector<Planeta>::iterator Uniwersum::znajdŸIterator(const Planeta & planeta)
{
	for (auto test = m_tablicaObiektów.begin(); test != m_tablicaObiektów.end(); test++)
	{
		if (&(*test) == &(planeta))
		{
			return test;
			break;
		}
	}
	return m_tablicaObiektów.end();
}

Vector2d Uniwersum::obliczSi³yGrawitacji(const Planeta &a)
{
	Vector2d sumaSi³ = { 0,0 };
	
	for (Planeta & b : m_tablicaObiektów)
	{
		if (&b != &a)
		{
			double wypadkowa = G_Sta³aGrawitacji * (a.m_masa * b.m_masa) / pow(a.odleg³oœæ(&b), 2);
			double alfa = atan2(b.m_pos.x - a.m_pos.x, b.m_pos.y - a.m_pos.y);
			sumaSi³.x += wypadkowa * sin(alfa);
			sumaSi³.y += wypadkowa * cos(alfa);
		}
	}
	return sumaSi³;
}

void Uniwersum::obliczPozycjê(Planeta &planeta, const double &czas)		//Musi byæ referencja, inaczej crash???
{
	if (planeta.m_zablokowana) return;

	Vector2d m_F = obliczSi³yGrawitacji(planeta);


	Vector2d przysp = m_F / planeta.m_masa;			//Ca³kowanie przybli¿one trapezami, dok³adne przy sta³ym przyspieszeniu

	planeta.m_pos.x += 0.5f * planeta.m_vel.x * czas;
	planeta.m_pos.y += 0.5f * planeta.m_vel.y * czas;

	planeta.m_vel += przysp * czas;

	planeta.m_pos.x += 0.5f * planeta.m_vel.x * czas;
	planeta.m_pos.y += 0.5f * planeta.m_vel.y * czas;

	planeta.setPosition(static_cast<float>(planeta.m_pos.x), static_cast<float>(planeta.m_pos.y));
}

void Uniwersum::kolizje()
{
	///////////////////Kolizje

	//Referencje do koliduj¹cych planet:
	Planeta * a = nullptr;
	Planeta * b = nullptr;
	//Flaga wyst¹pienia kolizji
	bool kolizja{ false };

	mu_tObiektów.lock();		//Blokada tablicy objektów - funkcja kolizji


	for (auto it_a = m_tablicaObiektów.begin(); it_a != m_tablicaObiektów.end(); it_a++)
	{
		for (auto it_b = it_a + 1; it_b != m_tablicaObiektów.end(); it_b++)
		{
			if ((*it_a).sprawdŸKolizjê(&(*(it_b))))
			{
				a = &(*it_a);
				b = &(*it_b);
				kolizja = true;
				break;
			}
		}
		if (kolizja) break;
	}

	mu_tObiektów.unlock();		//Teoretycznie trzeba odblokowaæ dopiero po porównaniu mas
								//Bêdzie krasz je¿eli w tym momencie zostanie usuniêta jakaœ planeta

	if (kolizja && a && b)
	{
		//Usuñ mniej masywn¹ lub obie
		if (a->m_masa > b->m_masa)
			usuñPlanetê(*b);
		else if (a->m_masa < b->m_masa)
			usuñPlanetê(*a);
		else
		{
			usuñPlanetê(*a);
			usuñPlanetê(*b);
		}
		//std::cout << "Wykryto kolizjê.\n";
	}
}