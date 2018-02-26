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


void fRysowania(sf::RenderWindow &window, std::vector<Obiekt*> &tablicaObiektów)
{
	window.clear(sf::Color::Black);

	for (Obiekt *obiekt : tablicaObiektów)
	{
		window.draw(obiekt->m_circle);
	}

	mu_tŚladów.lock();
	for (auto *ślad : tablicaŚladów)
	{
		window.draw(*ślad);
	}
	mu_tŚladów.unlock();

	window.display();
}

void t_Fizyka(const sf::RenderWindow *window, const std::vector<Obiekt*> *tablicaObiektów)
{
	long long numerCyklu{ 0 };
	sf::Clock clock;
	sf::Clock clock2;
	while (window->isOpen())
	{		
		mu_tObiektów.lock();
		sf::Time czas = clock.restart();
		for (auto obiekt : *tablicaObiektów)
		{
			obiekt->obliczPozycję(czas);
		}
		mu_tObiektów.unlock();
		std::cout << numerCyklu++ << " cykl fizyki zajął: " << clock2.restart().asMicroseconds() << " us.\n";
		//std::this_thread::sleep_for(std::chrono::milliseconds(1)); //TODO coś kraszuje bez tego, ten wątek chce odświeżyć niekompletny obiekt?
	}
}

void t_Ślady(const sf::RenderWindow *window, const std::vector<Obiekt*> *tablicaObiektów)
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
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //TODO coś kraszuje bez tego, ten wątek chce odświeżyć niekompletny obiekt?
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(G_WINWIDTH, G_WINHEIGHT), "Moje okienko xDD",sf::Style::Default,settings);

	std::vector<Obiekt*> tablicaObiektów;
	std::vector<sf::VertexArray*> tablicaŚladów;

	//Obiekt planeta(&tablicaObiektów, 1, { 20,20 }, { 0,0 }, 10, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));

	PlanetaStała *słońce = new PlanetaStała (&tablicaObiektów, 4, { 20,16 }, { 0,0 }, 200, sf::Color::Yellow);
	Obiekt *planeta_testowa = new Obiekt(&tablicaObiektów, 1, { 20,26 }, { 10,0 }, 10, sf::Color::Yellow);
	//Wątek fizyki
	std::thread t1(t_Fizyka, &window, &tablicaObiektów);
	std::thread t2(t_Ślady, &window, &tablicaObiektów);

	sf::View view(sf::FloatRect(0, 0, 800, 600));
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