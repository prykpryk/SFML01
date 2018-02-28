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
	sf::Clock clock2;

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

		mu_tObiekt�w.lock();
		for (auto obiekt : m_tablicaObiekt�w)
		{
			obiekt->obliczPozycj�(czas);
		}
		mu_tObiekt�w.unlock();

		if ((numerCyklu % 10000000) == 0 && DEBUG)
			std::cout << "1e7 cykli fizyki przy " << m_tablicaObiekt�w.size() << " planetach zaj�o �rednio: " << clock2.restart().asMicroseconds() / 10000000.0 << " us.\n";

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
		mu_tRys.lock();
		window->clear(sf::Color::Black);

		for (Planeta *obiekt : m_tablicaObiekt�w)
		{
			//Rysuj planety
			window->draw(*obiekt);
			//Rysuj �lady
			window->draw(&(*(obiekt->m_�lad.begin())), obiekt->m_�lad.size(), sf::LineStrip);
		}

		window->display();
		mu_tRys.unlock();
	}
	window->setActive(false);
}

void Uniwersum::t�lady()
{
	sf::Clock clock;
	long long numerCyklu{ 0 };

	while (window->isOpen())
	{
		if (m_pr�dko��Symulacji <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		clock.restart();
		numerCyklu++;
		//mu_tObiekt�w.lock();  Najwyra�niej nie potrzebne, po usuni�ciu tego mutexa ten w�tek nie jest bardzo blokowany przez w�tek fizyki.
		for (auto obiekt : m_tablicaObiekt�w)
		{
			obiekt->od�wie��lad();
		}
		//mu_tObiekt�w.unlock();
		if ((numerCyklu % 100) == 0 && DEBUG)
			std::cout << numerCyklu << " cykl �lad�w zaja�: " << clock.restart().asMicroseconds() << " us.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
