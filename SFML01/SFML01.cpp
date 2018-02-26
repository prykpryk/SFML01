// SFML01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Stałe.h"

#include "Planeta.h"
#include "PlanetaStała.h"
#include "ZmienneGlob.h"
#include "f_Różne.h"
#include "f_UI.h"


void fRysowania(RenderWindow &window, std::vector<Planeta*> &tablicaObiektów)
{
	window.clear(Color::Black);

	for (Planeta *obiekt : tablicaObiektów)
	{
		window.draw(*obiekt);
	}

	//mu_tŚladów.lock();
	for (std::vector<Vertex> *ślad : tablicaŚladów)
	{
		window.draw(&(ślad->at(0)), ślad->size(), sf::LineStrip); //std::vector<Vertex>

		//window.draw(&(*(ślad->begin())), ślad->size(), sf::LineStrip);  //std::list<Vertex> - nie działa
	}
	//mu_tŚladów.unlock();

	window.display();
}

void t_Fizyka(const RenderWindow *window, const std::vector<Planeta*> *tablicaObiektów)
{
	long long numerCyklu{ 0 };
	Clock clock;
	Clock clock2;

	while (window->isOpen())
	{		
		numerCyklu++;


		double czas = clock.restart().asSeconds() * G_PrędkośćSymulacji;

		mu_tObiektów.lock();
		for (auto obiekt : *tablicaObiektów)
		{	
			obiekt->obliczPozycję(czas);
		}
		mu_tObiektów.unlock();

		if ((numerCyklu % 1000000) == 0 && DEBUG)
			std::cout << "1e6 cykli fizyki przy " << tablicaObiektów->size() << " planetach zajęło średnio: " << clock2.restart().asMicroseconds()/1000000.0 << " us.\n";

		//std::this_thread::sleep_for(std::chrono::microseconds(5));		//Dajmy czas na odblokowanie mutexa, dla stworzenia nowych obiektów itd.
		//jednak nie dawajmy, ten sleep działa na zbyt długi czas około 1ms

		//Benchmark w konf. DEBUG
		//Kopiowanie tablicy:	3 obj - 7.4us, 10 obj - 28.3us		Zaleta: mutex odblokowany po skopiowaniu tablicy (po ok 5us), też niestabilne jak kop. co 10
		//Bez kopiowania:		3 obj - 2.4us, 10 obj - 23us		Zaleta: dla 3 obj jest 3 razy szybciej, stabilne
		//Kop. tab. co 10 cykli: jak bez kopiowania					Wada: Niestabilne, nie wiadomo czy obiekt nie został usunięty.
	}
}

void t_Ślady(const RenderWindow *window, const std::vector<Planeta*> *tablicaObiektów)
{
	Clock clock;
	long long numerCyklu{ 0 };

	while (window->isOpen())
	{
		clock.restart();
		numerCyklu++;
		//mu_tObiektów.lock();  Najwyraźniej nie potrzebne, po usunięciu tego mutexa ten wątek nie jest bardzo blokowany przez wątek fizyki.
		for (auto obiekt : *tablicaObiektów)
		{
			obiekt->odświeżŚlad();
		}
		//mu_tObiektów.unlock();
		if ((numerCyklu % 10000) == 0 && DEBUG)
			std::cout << numerCyklu << " cykl śladów zajał: " << clock.restart().asMicroseconds() << " us.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}

int main()
{
	std::setlocale(LC_ALL, "");
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(G_WINWIDTH, G_WINHEIGHT), "Moje okienko xDD",Style::Default,settings);

	std::vector<Planeta*> tablicaObiektów;

	//Obiekt planeta(&tablicaObiektów, 1, { 20,20 }, { 0,0 }, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));

	PlanetaStała *słońce = new PlanetaStała (&tablicaObiektów, 10, { 0,0 }, { 0,0 }, 1000, Color::Yellow);
	Planeta *planeta_testowa = new Planeta(&tablicaObiektów, 1, { 0,30 }, { 13,0 }, 1, Color::Cyan);
	Planeta *księżyc_testowa = new Planeta(&tablicaObiektów, 0.3, { 0,31 }, { 15,0 }, pow(0.3,3), Color::White);

	//Wątek fizyki
	std::thread t1(t_Fizyka, &window, &tablicaObiektów);
	std::thread t2(t_Ślady, &window, &tablicaObiektów);

	View view;
	view.setCenter(0, 0);
	view.setSize(4 * 800, 4 * 600);
	window.setView(view);

	instrukcja();

	while (window.isOpen())
	{
		handleEvents(window, tablicaObiektów);

		fRysowania(window, tablicaObiektów);
	}
	
	t1.join();
	t2.join();

	return 0;
}