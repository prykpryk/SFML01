//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"

#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_R�ne.h"


void Uniwersum::tFizyka()
{
	long long numerCyklu{ 0 };
	sf::Clock clock;
	sf::Clock clock_b;		//Benchmark

	while (window->isOpen())
	{
		if (m_pr�dko��Symulacji <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			clock.restart();
			continue;
		}

		numerCyklu++;

		double czas = clock.restart().asSeconds() * m_pr�dko��Symulacji;

		mu_tObiekt�w.lock();		//BLOKADA TABLICY OBJEKT�W W�TEK FIZYKI
		for (Planeta & obiekt : m_tablicaObiekt�w)
		{
			obliczPozycj�(obiekt, czas);
		}
		mu_tObiekt�w.unlock();

		//kolizje();

		if (clock_b.getElapsedTime().asSeconds() > 10 && DEBUG)
		{
			std::cout << "P�tla fizyki trwa�a �rednio " << static_cast<double>(clock_b.restart().asMicroseconds()) / numerCyklu << " us przy " << m_tablicaObiekt�w.size() <<" planetach.\n";
			numerCyklu = 0;
		}

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
	window->setActive(true);
	while (window->isOpen())
	{

		window->clear(sf::Color::Black);


		mu_tObiekt�w.lock();		//Blokada tablicy objekt�w - w�tek rysowania
		std::vector<Planeta> kopiaUniwersum = m_tablicaObiekt�w;
		mu_tObiekt�w.unlock();

		for (Planeta & obiekt : kopiaUniwersum)
		{
			//Rysuj planety
			window->draw(obiekt);
			//Rysuj �lady
			window->draw(&(*(obiekt.m_�lad.begin())), obiekt.m_�lad.size(), sf::LineStrip);
		}

		window->display();

	}
	window->setActive(false);
}

void Uniwersum::t�lady()
{
	sf::Clock clock_B;		//Benchmark
	long long numerCyklu{ 0 };

	while (window->isOpen())
	{
		if (m_pr�dko��Symulacji <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		clock_B.restart();
		numerCyklu++;

		mu_tObiekt�w.lock();	//TODO: Przej�cie blokady zajmuje duuu�o czasu w tym miejscu, wymy�li� jak�� alternatyw�?
		for (Planeta & obiekt : m_tablicaObiekt�w)
		{
			obiekt.od�wie��lad();
		}
		mu_tObiekt�w.unlock();

		
		if (numerCyklu % 100 == 0 && DEBUG)
			std::cout << "P�tla �lad�w trwa�a " << clock_B.restart().asMicroseconds() << "us przy " << m_tablicaObiekt�w.size() << " planetach.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
