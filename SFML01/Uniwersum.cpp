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
	mu_tObiekt�w.lock();		//Blokada tablicy objekt�w - tworzenie planety
	m_tablicaObiekt�w.emplace_back(masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);
	mu_tObiekt�w.unlock();

	return &(*(m_tablicaObiekt�w.end() - 1));;
}

Planeta* Uniwersum::dodajPlanet�(
	const double masa,
	const Vector2d pos,
	const Vector2d vel,
	const sf::Color color,
	bool zablokowana)
{
	double r = obliczPromie�(masa);

	mu_tObiekt�w.lock();		//Blokada tablicy objekt�w - tworzenie planety
	m_tablicaObiekt�w.emplace_back(masa, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		color, zablokowana);
	mu_tObiekt�w.unlock();

	return &(*(m_tablicaObiekt�w.end()-1));
}

double Uniwersum::obliczPromie�(double masa)
{
	return cbrt(masa);
}

void Uniwersum::usu�Planet�(Planeta & planeta)
{
	if (&planeta == m_�ledzonaPlaneta)
		m_�ledzonaPlaneta = nullptr;

	/*for (auto it = tablica�lad�w.begin(); it != tablica�lad�w.end(); )
	{
		if (*it == &(planeta->m_�lad))
			it = tablica�lad�w.erase(it);
		else
			it++;
	}
	*/

	mu_tObiekt�w.lock();		//Blokada tablicy objekt�w - usuwanie planety

	auto it = znajd�Iterator(planeta);

	if (it != m_tablicaObiekt�w.end())
		m_tablicaObiekt�w.erase(it);

	mu_tObiekt�w.unlock();

}

std::vector<Planeta>::iterator Uniwersum::znajd�Iterator(const Planeta & planeta)
{
	for (auto test = m_tablicaObiekt�w.begin(); test != m_tablicaObiekt�w.end(); test++)
	{
		if (&(*test) == &(planeta))
		{
			return test;
			break;
		}
	}
	return m_tablicaObiekt�w.end();
}

Vector2d Uniwersum::obliczSi�yGrawitacji(const Planeta &a)
{
	Vector2d sumaSi� = { 0,0 };
	
	for (Planeta & b : m_tablicaObiekt�w)
	{
		if (&b != &a)
		{
			double wypadkowa = G_Sta�aGrawitacji * (a.m_masa * b.m_masa) / pow(a.odleg�o��(&b), 2);
			double alfa = atan2(b.m_pos.x - a.m_pos.x, b.m_pos.y - a.m_pos.y);
			sumaSi�.x += wypadkowa * sin(alfa);
			sumaSi�.y += wypadkowa * cos(alfa);
		}
	}
	return sumaSi�;
}

void Uniwersum::obliczPozycj�(Planeta &planeta, const double &czas)		//Musi by� referencja, inaczej crash???
{
	if (planeta.m_zablokowana) return;

	Vector2d m_F = obliczSi�yGrawitacji(planeta);


	Vector2d przysp = m_F / planeta.m_masa;			//Ca�kowanie przybli�one trapezami, dok�adne przy sta�ym przyspieszeniu

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

	//Referencje do koliduj�cych planet:
	Planeta * a = nullptr;
	Planeta * b = nullptr;
	//Flaga wyst�pienia kolizji
	bool kolizja{ false };

	mu_tObiekt�w.lock();		//Blokada tablicy objekt�w - funkcja kolizji


	for (auto it_a = m_tablicaObiekt�w.begin(); it_a != m_tablicaObiekt�w.end(); it_a++)
	{
		for (auto it_b = it_a + 1; it_b != m_tablicaObiekt�w.end(); it_b++)
		{
			if ((*it_a).sprawd�Kolizj�(&(*(it_b))))
			{
				a = &(*it_a);
				b = &(*it_b);
				kolizja = true;
				break;
			}
		}
		if (kolizja) break;
	}

	mu_tObiekt�w.unlock();		//Teoretycznie trzeba odblokowa� dopiero po por�wnaniu mas
								//B�dzie krasz je�eli w tym momencie zostanie usuni�ta jaka� planeta

	if (kolizja && a && b)
	{
		//Usu� mniej masywn� lub obie
		if (a->m_masa > b->m_masa)
			usu�Planet�(*b);
		else if (a->m_masa < b->m_masa)
			usu�Planet�(*a);
		else
		{
			usu�Planet�(*a);
			usu�Planet�(*b);
		}
		//std::cout << "Wykryto kolizj�.\n";
	}
}