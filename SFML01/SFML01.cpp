// SFML01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Stałe.h"

#include "Planeta.h"
#include "PlanetaStała.h"
#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_Różne.h"
#include "f_UI.h"

int main()
{
	std::setlocale(LC_ALL, "");

	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(G_WINWIDTH, G_WINHEIGHT), "Moje okienko xDD",Style::Default,settings);

	Uniwersum uni(&window);

	//Wątki
	std::thread t1(&Uniwersum::tFizyka, &uni);
	std::thread t2(&Uniwersum::tŚlady, &uni);

	window.setActive(false);
	std::thread t3(&Uniwersum::tRysowanie2, &uni);


	//Planety domyślne
	PlanetaStała *słońce = uni.dodajPlanetęStałą(10, { 0,0 }, { 0,0 }, 1000, Color::Yellow);
	Planeta *planeta_testowa = uni.dodajPlanetę(1, { 0,30.0 }, { 13.0,0 }, 1, Color::Cyan);
	Planeta *księżyc_testowa = uni.dodajPlanetę(0.3, { 0,31.0 }, { 15.0,0 }, pow(0.3, 3), Color::White);


	//Widok początkowy
	View view;
	view.setCenter(0, 0);
	view.setSize(4 * 800, 4 * 600);
	window.setView(view);

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