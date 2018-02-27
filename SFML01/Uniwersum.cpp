#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"
#include "PlanetaSta³a.h"
#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"
#include "f_UI.h"

Planeta* Uniwersum::dodajPlanetê(
	const double r,
	const Vector2d pos,
	const Vector2d vel,
	const double masa,
	const Color color)
{
	auto ptr = new Planeta(&m_tablicaObiektów, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		masa, color);

	mu_tObiektów.lock();
	m_tablicaObiektów.push_back(ptr);
	mu_tObiektów.unlock();

	return ptr;
}

PlanetaSta³a* Uniwersum::dodajPlanetêSta³¹(
	const double r,
	const Vector2d pos,
	const Vector2d vel,
	const double masa,
	const Color color)
{
	auto ptr = new PlanetaSta³a(&m_tablicaObiektów, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		masa, color);
	mu_tObiektów.lock();
	m_tablicaObiektów.push_back(ptr);
	mu_tObiektów.unlock();

	return ptr;
}

void Uniwersum::wybierzRozmiar(int i)
{
	m_wybranaWielkosæ += i;
	std::cout << "Wybrano masê planety " << pow(10.0, m_wybranaWielkosæ) << " kg.\n";
}

void Uniwersum::wybierzPrêdkoœæSymulacji(double i)
{
	m_prêdkoœæSymulacji *= i;
	std::cout << "Wybrano prêdkoœæ symulacji " << m_prêdkoœæSymulacji << "x.\n";
}


void Uniwersum::tFizyka()
{
	long long numerCyklu{ 0 };
	Clock clock;
	Clock clock2;

	while (window->isOpen())
	{
		numerCyklu++;

		double czas = clock.restart().asSeconds() * m_prêdkoœæSymulacji;

		mu_tObiektów.lock();
		for (auto obiekt : m_tablicaObiektów)
		{
			obiekt->obliczPozycjê(czas);
		}
		mu_tObiektów.unlock();

		if ((numerCyklu % 1000000) == 0 && DEBUG)
			std::cout << "1e6 cykli fizyki przy " << m_tablicaObiektów.size() << " planetach zajê³o œrednio: " << clock2.restart().asMicroseconds() / 1000000.0 << " us.\n";

		//std::this_thread::sleep_for(std::chrono::microseconds(5));		//Dajmy czas na odblokowanie mutexa, dla stworzenia nowych obiektów itd.
		//jednak nie dawajmy, ten sleep dzia³a na zbyt d³ugi czas oko³o 1ms

		//Benchmark w konf. DEBUG
		//Kopiowanie tablicy:	3 obj - 7.4us, 10 obj - 28.3us		Zaleta: mutex odblokowany po skopiowaniu tablicy (po ok 5us), te¿ niestabilne jak kop. co 10
		//Bez kopiowania:		3 obj - 2.4us, 10 obj - 23us		Zaleta: dla 3 obj jest 3 razy szybciej, stabilne
		//Kop. tab. co 10 cykli: jak bez kopiowania					Wada: Niestabilne, nie wiadomo czy obiekt nie zosta³ usuniêty.
	}
}

void Uniwersum::tRysowanie()
{
	window->clear(Color::Black);

	for (Planeta *obiekt : m_tablicaObiektów)
	{
		window->draw(*obiekt);
	}

	for (std::vector<Vertex> *œlad : tablicaŒladów)
	{
		window->draw(&(œlad->at(0)), œlad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia³a
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

		for (Planeta *obiekt : m_tablicaObiektów)
		{
			window->draw(*obiekt);
		}

		for (std::vector<Vertex> *œlad : tablicaŒladów)
		{
			window->draw(&(œlad->at(0)), œlad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia³a
		}

		window->display();
		mu_tRys.unlock();
	}
	window->setActive(false);
}

void Uniwersum::tŒlady()
{
	Clock clock;
	long long numerCyklu{ 0 };

	while (window->isOpen())
	{
		clock.restart();
		numerCyklu++;
		//mu_tObiektów.lock();  NajwyraŸniej nie potrzebne, po usuniêciu tego mutexa ten w¹tek nie jest bardzo blokowany przez w¹tek fizyki.
		for (auto obiekt : m_tablicaObiektów)
		{
			obiekt->odœwie¿Œlad();
		}
		//mu_tObiektów.unlock();
		if ((numerCyklu % 10000) == 0 && DEBUG)
			std::cout << numerCyklu << " cykl œladów zaja³: " << clock.restart().asMicroseconds() << " us.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
