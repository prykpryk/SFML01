#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"
#include "PlanetaSta�a.h"
#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_R�ne.h"
#include "f_UI.h"

Planeta* Uniwersum::dodajPlanet�(
	const double r,
	const Vector2d pos,
	const Vector2d vel,
	const double masa,
	const Color color)
{
	auto ptr = new Planeta(&m_tablicaObiekt�w, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		masa, color);

	mu_tObiekt�w.lock();
	m_tablicaObiekt�w.push_back(ptr);
	mu_tObiekt�w.unlock();

	return ptr;
}

PlanetaSta�a* Uniwersum::dodajPlanet�Sta��(
	const double r,
	const Vector2d pos,
	const Vector2d vel,
	const double masa,
	const Color color)
{
	auto ptr = new PlanetaSta�a(&m_tablicaObiekt�w, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		masa, color);
	mu_tObiekt�w.lock();
	m_tablicaObiekt�w.push_back(ptr);
	mu_tObiekt�w.unlock();

	return ptr;
}

void Uniwersum::wybierzRozmiar(int i)
{
	m_wybranaWielkos� += i;
	std::cout << "Wybrano mas� planety " << pow(10.0, m_wybranaWielkos�) << " kg.\n";
}

void Uniwersum::wybierzPr�dko��Symulacji(double i)
{
	m_pr�dko��Symulacji *= i;
	std::cout << "Wybrano pr�dko�� symulacji " << m_pr�dko��Symulacji << "x.\n";
}


void Uniwersum::tFizyka()
{
	long long numerCyklu{ 0 };
	Clock clock;
	Clock clock2;

	while (window->isOpen())
	{
		numerCyklu++;

		double czas = clock.restart().asSeconds() * m_pr�dko��Symulacji;

		mu_tObiekt�w.lock();
		for (auto obiekt : m_tablicaObiekt�w)
		{
			obiekt->obliczPozycj�(czas);
		}
		mu_tObiekt�w.unlock();

		if ((numerCyklu % 1000000) == 0 && DEBUG)
			std::cout << "1e6 cykli fizyki przy " << m_tablicaObiekt�w.size() << " planetach zaj�o �rednio: " << clock2.restart().asMicroseconds() / 1000000.0 << " us.\n";

		//std::this_thread::sleep_for(std::chrono::microseconds(5));		//Dajmy czas na odblokowanie mutexa, dla stworzenia nowych obiekt�w itd.
		//jednak nie dawajmy, ten sleep dzia�a na zbyt d�ugi czas oko�o 1ms

		//Benchmark w konf. DEBUG
		//Kopiowanie tablicy:	3 obj - 7.4us, 10 obj - 28.3us		Zaleta: mutex odblokowany po skopiowaniu tablicy (po ok 5us), te� niestabilne jak kop. co 10
		//Bez kopiowania:		3 obj - 2.4us, 10 obj - 23us		Zaleta: dla 3 obj jest 3 razy szybciej, stabilne
		//Kop. tab. co 10 cykli: jak bez kopiowania					Wada: Niestabilne, nie wiadomo czy obiekt nie zosta� usuni�ty.
	}
}

void Uniwersum::tRysowanie()
{
	window->clear(Color::Black);

	for (Planeta *obiekt : m_tablicaObiekt�w)
	{
		window->draw(*obiekt);
	}

	for (std::vector<Vertex> *�lad : tablica�lad�w)
	{
		window->draw(&(�lad->at(0)), �lad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia�a
	}

	window->display();
}

void Uniwersum::tRysowanie2()
{
	window->setActive(true);
	while (window->isOpen())
	{
		mu_tRys.lock();
		window->clear(Color::Black);

		for (Planeta *obiekt : m_tablicaObiekt�w)
		{
			window->draw(*obiekt);
		}

		for (std::vector<Vertex> *�lad : tablica�lad�w)
		{
			window->draw(&(�lad->at(0)), �lad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia�a
		}

		window->display();
		mu_tRys.unlock();
	}
	window->setActive(false);
}

void Uniwersum::t�lady()
{
	Clock clock;
	long long numerCyklu{ 0 };

	while (window->isOpen())
	{
		clock.restart();
		numerCyklu++;
		//mu_tObiekt�w.lock();  Najwyra�niej nie potrzebne, po usuni�ciu tego mutexa ten w�tek nie jest bardzo blokowany przez w�tek fizyki.
		for (auto obiekt : m_tablicaObiekt�w)
		{
			obiekt->od�wie��lad();
		}
		//mu_tObiekt�w.unlock();
		if ((numerCyklu % 10000) == 0 && DEBUG)
			std::cout << numerCyklu << " cykl �lad�w zaja�: " << clock.restart().asMicroseconds() << " us.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
