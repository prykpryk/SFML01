// SFML01.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Stałe.h"

#include "Planeta.h"
#include "ZmienneGlob.h"
#include "f_Różne.h"
#include "f_UI.h"
#include "Uniwersum.h"


void scen_PlanetyWewnętrzne(Uniwersum & uni)
{
	//Planety domyślne
	uni.dodajPlanetę(1000, { 0,0 }, { 0,0 }, sf::Color::Yellow, true);				//Słońce
	uni.dodajPlanetę(0.1, { 0,25.0 }, { 14,0 }, sf::Color(100, 100, 100));			//Merkury
	uni.dodajPlanetę(8.5, { 0,50.0 }, { 10,0 }, sf::Color(211, 216, 149));			//Wenus
	Planeta* ziemia = uni.dodajPlanetę(10, { 0,100.0 }, { 7,0 }, sf::Color::Cyan);					//Ziemia
	uni.dodajPlanetę(0.027, { 0,105.0 }, { 10,0 }, sf::Color::White);				//Księżyc
	uni.dodajPlanetę(0.1, { 0, 200 }, { 5,0 }, sf::Color(160, 103, 46));			//Mars

	uni.ustawŚledzonąPlanetę(ziemia);
}

void scen_2Słońca1Planeta(Uniwersum & uni)			//xDDDDDDDDDDDD
{
	//Planety domyślne
	uni.dodajPlanetę(1000, { -50, 0 }, { 0,0 }, sf::Color::Yellow, true);
	uni.dodajPlanetę(1000, { 50, 0 }, { 0,0 }, sf::Color::Yellow, true);
	uni.dodajPlanetę(10, { -50, 30 }, { 15,0 }, sf::Color::Red, false);

}

int main()
{
	std::setlocale(LC_ALL, "");

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(G_WINWIDTH, G_WINHEIGHT), "Planetki", sf::Style::Default,settings);
	window.setActive(false);

	Uniwersum uni(&window);

	scen_2Słońca1Planeta(uni);

	//Wątki
	std::thread t1(&Uniwersum::tFizyka, &uni);
	std::thread t2(&Uniwersum::tŚlady, &uni);
	std::thread t3(&Uniwersum::tRysowanie2, &uni);

	//Widok początkowy
	sf::View view;
	view.setCenter(0, 0);
	view.setSize(G_WINWIDTH / 4.0f, G_WINHEIGHT / 4.0f);
	window.setView(view);

	//Wyświetl klawiszologię
	instrukcja();

	while (window.isOpen())
	{
		uni.tHandleEvents();
		uni.tŚledźPlanetę();
	}
	
	t1.join();
	t2.join();
	t3.join();

	return 0;
}