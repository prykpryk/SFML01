//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"

#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"


void Uniwersum::little_sleep(std::chrono::microseconds us)
{
	auto start = std::chrono::high_resolution_clock::now();
	auto end = start + us;
	do {
		std::this_thread::yield();
	} while (std::chrono::high_resolution_clock::now() < end);
}

void Uniwersum::tFizyka()
{
	long long numerCyklu{ 0 };
	sf::Clock clock;
	sf::Clock clock_b;		//Benchmark

	while (window->isOpen())
	{
		if (m_prêdkoœæSymulacji <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
			clock.restart();
			continue;
		}

		numerCyklu++;

		double czas = clock.restart().asSeconds() * m_prêdkoœæSymulacji;

		mu_tObiektów.lock();		//BLOKADA TABLICY OBJEKTÓW W¥TEK FIZYKI
		for (Planeta & obiekt : m_tablicaObiektów)
		{
			obliczPozycjê(obiekt, czas);
		}
		mu_tObiektów.unlock();

		//kolizje();

		if (clock_b.getElapsedTime().asSeconds() > 5 && DEBUG)
		{
			std::cout << numerCyklu << "pêtli fizyki trwa³o œrednio " << static_cast<double>(clock_b.restart().asMicroseconds()) / numerCyklu << " us przy " << m_tablicaObiektów.size() <<" planetach.\n";
			numerCyklu = 0;
		}

		little_sleep(std::chrono::microseconds(5));		//Dajmy czas na odblokowanie mutexa, dla stworzenia nowych obiektów itd.
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
	long long numerCyklu{ 0 };

	sf::Clock clock_b;		//Benchmark
	while (window->isOpen())
	{
		numerCyklu++;
		clock_b.restart();
		window->clear(sf::Color::Black);
		

		mu_tObiektów.lock();		//Blokada tablicy objektów - w¹tek rysowania
		std::vector<Planeta> kopiaUniwersum = m_tablicaObiektów;
		mu_tObiektów.unlock();

		for (Planeta & obiekt : kopiaUniwersum)
		{
			//Rysuj planety
			window->draw(obiekt);
			//Rysuj œlady
			window->draw(&(*(obiekt.m_œlad.begin())), obiekt.m_œlad.size(), sf::LineStrip);
		}

		window->display();

		kolizje();

		if (numerCyklu % 1000 == 0 && DEBUG)
		{
			std::cout << "Pêtla rysowania trwa³a " << static_cast<double>(clock_b.restart().asMilliseconds()) << " ms przy " << m_tablicaObiektów.size() << " planetach.\n";
			numerCyklu = 0;
		}

	}
	window->setActive(false);
}

void Uniwersum::tŒlady()
{
	sf::Clock clock_B;		//Benchmark
	long long numerCyklu{ 0 };

	while (window->isOpen())
	{
		if (m_prêdkoœæSymulacji <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		clock_B.restart();
		numerCyklu++;

		mu_tObiektów.lock();	//TODO: Przejêcie blokady zajmuje duuu¿o czasu w tym miejscu, wymyœliæ jak¹æ alternatywê?
		for (Planeta & obiekt : m_tablicaObiektów)
		{
			obiekt.odœwie¿Œlad();
		}
		mu_tObiektów.unlock();

		
		if (numerCyklu % 100 == 0 && DEBUG)
			std::cout << "Pêtla œladów trwa³a " << clock_B.restart().asMicroseconds() << "us przy " << m_tablicaObiektów.size() << " planetach.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
