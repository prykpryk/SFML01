// SFML01.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Stałe.h"

#include "Planeta.h"
#include "PlanetaStała.h"
#include "ZmienneGlob.h"
#include "f_Różne.h"
#include "f_UI.h"
#include "Uniwersum.h"


int main()
{
	std::setlocale(LC_ALL, "");

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(G_WINWIDTH, G_WINHEIGHT), "Planetki", sf::Style::Default,settings);
	window.setActive(false);

	Uniwersum uni(&window);

	//Planety domyślne
	Planeta *słońce = uni.dodajPlanetęStałą(10, { 0,0 }, { 0,0 }, 1000, sf::Color::Yellow);
	Planeta *planeta_testowa = uni.dodajPlanetę(cbrt(10), { 0,50.0 }, { 10.0,0 }, 10, sf::Color::Cyan);
	Planeta *księżyc_testowa = uni.dodajPlanetę(0.3, { 0,53.0 }, { 14.5,0 }, pow(0.3, 3), sf::Color::White);


	//Wątki
	std::thread t1(&Uniwersum::tFizyka, &uni);
	std::thread t2(&Uniwersum::tŚlady, &uni);
	std::thread t3(&Uniwersum::tRysowanie2, &uni);

	//Widok początkowy
	sf::View view;
	view.setCenter(0, 0);
	view.setSize(4 * 800, 4 * 600);
	window.setView(view);

	//Wyświetl klawiszologię
	instrukcja();

	while (window.isOpen())
	{
		uni.tHandleEvents();
	}
	
	t1.join();
	t2.join();
	t3.join();

	return 0;
}