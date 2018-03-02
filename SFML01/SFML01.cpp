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
#include "Scenariusze.h"
#include "GUI.h"

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
	scen_dużoPlanetek(uni);

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