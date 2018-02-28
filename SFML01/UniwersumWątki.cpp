//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"

#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"


void Uniwersum::tFizyka()
{
	long long numerCyklu{ 0 };
	sf::Clock clock;
	sf::Clock clock2;

	while (window->isOpen())
	{
		if (m_prêdkoœæSymulacji <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			clock.restart();
			continue;
		}

		numerCyklu++;

		double czas = clock.restart().asSeconds() * m_prêdkoœæSymulacji;

		mu_tObiektów.lock();
		for (auto obiekt : m_tablicaObiektów)
		{
			obiekt->obliczPozycjê(czas);
		}
		mu_tObiektów.unlock();

		if ((numerCyklu % 10000000) == 0 && DEBUG)
			std::cout << "1e7 cykli fizyki przy " << m_tablicaObiektów.size() << " planetach zajê³o œrednio: " << clock2.restart().asMicroseconds() / 10000000.0 << " us.\n";

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
	window->setActive(true);
	while (window->isOpen())
	{
		mu_tRys.lock();
		window->clear(sf::Color::Black);

		for (Planeta *obiekt : m_tablicaObiektów)
		{
			//Rysuj planety
			window->draw(*obiekt);
			//Rysuj œlady
			window->draw(&(*(obiekt->m_œlad.begin())), obiekt->m_œlad.size(), sf::LineStrip);
		}

		window->display();
		mu_tRys.unlock();
	}
	window->setActive(false);
}

void Uniwersum::tŒlady()
{
	sf::Clock clock;
	long long numerCyklu{ 0 };

	while (window->isOpen())
	{
		if (m_prêdkoœæSymulacji <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		clock.restart();
		numerCyklu++;
		//mu_tObiektów.lock();  NajwyraŸniej nie potrzebne, po usuniêciu tego mutexa ten w¹tek nie jest bardzo blokowany przez w¹tek fizyki.
		for (auto obiekt : m_tablicaObiektów)
		{
			obiekt->odœwie¿Œlad();
		}
		//mu_tObiektów.unlock();
		if ((numerCyklu % 100) == 0 && DEBUG)
			std::cout << numerCyklu << " cykl œladów zaja³: " << clock.restart().asMicroseconds() << " us.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
