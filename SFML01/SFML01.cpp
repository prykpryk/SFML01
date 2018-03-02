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
	Planeta* ziemia = uni.dodajPlanetę(10, { 0,100.0 }, { 7,0 }, sf::Color::Cyan);	//Ziemia
	uni.dodajPlanetę(0.027, { 0,105.0 }, { 10,0 }, sf::Color::White);				//Księżyc
	uni.dodajPlanetę(0.1, { 0, 200 }, { 5,0 }, sf::Color(160, 103, 46));			//Mars
}

void scen_2Słońca1Planeta(Uniwersum & uni)			//xDDDDDDDDDDDD
{
	//Planety domyślne
	uni.dodajPlanetę(1000, 5, { -50, 0 }, { 0,0 }, sf::Color::Yellow, true);
	uni.dodajPlanetę(1000, 5, { 50, 0 }, { 0,0 }, sf::Color::Yellow, true);
	uni.dodajPlanetę(10, { 0, 0 }, { 7.35, 9 }, sf::Color::Red, false);
	uni.dodajPlanetę(0.1, { 0, 100 }, { 9, 0 }, sf::Color::Blue, false);
	uni.dodajPlanetę(0.1, { -50, 8 }, { 24, 0 }, sf::Color::White, false);
}

void scen_UkłBinarny(Uniwersum & uni)
{
	//Planety domyślne
	uni.dodajPlanetę(1000000, { -150, 0 }, { 0,100 }, sf::Color::Cyan);
	uni.dodajPlanetę(1000000, { 150, 0 }, { 0,-100 }, sf::Color::Yellow);
	uni.dodajPlanetę(1000, { 3000, 0 }, { 0, -57 }, sf::Color::White);
	uni.dodajPlanetę(1, { 3020, 0 }, { 0, -74 }, sf::Color::Green);
	uni.dodajPlanetę(0.1, { 800, 0 }, { 0, -120 }, sf::Color::Red);
}

void wylosujScenariusz(Uniwersum & uni)
{
	switch (rand(1, 3))
	{
	case 1:
		scen_PlanetyWewnętrzne(uni); break;
	case 2:
		scen_2Słońca1Planeta(uni); break;
	case 3:
		scen_UkłBinarny(uni); break;
	default:
		scen_PlanetyWewnętrzne(uni); break;
	}
}

int main()
{
	std::setlocale(LC_ALL, "");

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(G_WINWIDTH, G_WINHEIGHT), "Planetki", sf::Style::Default,settings);
	window.setVerticalSyncEnabled(true);
	window.setActive(false);

	Uniwersum uni(&window);


	//Scenariusz początkowy
	scen_UkłBinarny(uni);

	//Wątki
	std::thread t1(&Uniwersum::tFizyka, &uni);
	std::thread t2(&Uniwersum::tŚlady, &uni);
	std::thread t3(&Uniwersum::tRysowanie, &uni);

	//Widok początkowy
	sf::View view;
	view.setCenter(0, 0);
	view.setSize(G_WINWIDTH / 4.0f, G_WINHEIGHT / 4.0f);
	window.setView(view);
	uni.odświeżKształy();


	//Wyświetl klawiszologię
	instrukcja();

	while (window.isOpen())
	{
		uni.handleEvents();
		uni.śledźPlanetę();
	}
	
	t1.join();
	t2.join();
	t3.join();

	return 0;
}