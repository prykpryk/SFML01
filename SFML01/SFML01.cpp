// SFML01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Stałe.h"

#include "Obiekt.h"
#include "PlanetaStała.h"
#include "ZmienneGlob.h"
#include "f_Różne.h"
#include "f_UI.h"


void fRysowania(RenderWindow &window, std::vector<Obiekt*> &tablicaObiektów)
{
	window.clear(Color::Black);

	for (Obiekt *obiekt : tablicaObiektów)
	{
		window.draw(obiekt->m_circle);
	}

	mu_tŚladów.lock();
	for (std::vector<Vertex> *ślad : tablicaŚladów)
	{
		window.draw(&(ślad->at(0)), ślad->size(), sf::LineStrip);
	}
	mu_tŚladów.unlock();

	window.display();
}

void t_Fizyka(const RenderWindow *window, const std::vector<Obiekt*> *tablicaObiektów)
{
	long long numerCyklu{ 0 };
	Clock clock;
	Clock clock2;
	while (window->isOpen())
	{		
		mu_tObiektów.lock();
		Time czas = clock.restart();
		for (auto obiekt : *tablicaObiektów)
		{
			obiekt->obliczPozycję(czas);
		}
		mu_tObiektów.unlock();
		//std::cout << numerCyklu++ << " cykl fizyki zajal: " << clock2.restart().asMicroseconds() << " us.\n";
		//std::this_thread::sleep_for(std::chrono::milliseconds(1)); //TODO coś kraszuje bez tego, ten wątek chce odświeżyć niekompletny obiekt?
	}
}

void t_Ślady(const RenderWindow *window, const std::vector<Obiekt*> *tablicaObiektów)
{
	while (window->isOpen())
	{
		mu_tObiektów.lock();
		for (auto obiekt : *tablicaObiektów)
		{
			obiekt->odświeżŚlad();
		}
		mu_tObiektów.unlock();
		//std::cout << "Ten cykl fizyki zajął: " << clock2.restart().asMicroseconds() << " us.\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(G_WINWIDTH, G_WINHEIGHT), "Moje okienko xDD",Style::Default,settings);

	std::vector<Obiekt*> tablicaObiektów;
	std::vector<VertexArray*> tablicaŚladów;

	//Obiekt planeta(&tablicaObiektów, 1, { 20,20 }, { 0,0 }, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));

	PlanetaStała *słońce = new PlanetaStała (&tablicaObiektów, 4, { 20,16 }, { 0,0 }, 200, Color::Yellow);
	Obiekt *planeta_testowa = new Obiekt(&tablicaObiektów, 1, { 20,26 }, { 10,0 }, 10, Color::Cyan);
	Obiekt *księżyc_testowa = new Obiekt(&tablicaObiektów, 0.2, { 20,27.5 }, { 15,0 }, 0.5, Color::White);

	//Wątek fizyki
	std::thread t1(t_Fizyka, &window, &tablicaObiektów);
	std::thread t2(t_Ślady, &window, &tablicaObiektów);

	View view(FloatRect(0, 0, 800, 600));
	window.setView(view);


	while (window.isOpen())
	{
		handleEvents(window, tablicaObiektów);

		fRysowania(window, tablicaObiektów);
	}
	
	t1.join();
	t2.join();

	return 0;
}