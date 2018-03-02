
#include <math.h>
#include "Uniwersum.h"


void scen_PlanetyWewn�trzne(Uniwersum & uni)
{
	//Planety domy�lne
	uni.dodajPlanet�(1000, { 0,0 }, { 0,0 }, sf::Color::Yellow, true);				//S�o�ce
	uni.dodajPlanet�(0.1, { 0,25.0 }, { 14,0 }, sf::Color(100, 100, 100));			//Merkury
	uni.dodajPlanet�(8.5, { 0,50.0 }, { 10,0 }, sf::Color(211, 216, 149));			//Wenus
	Planeta* ziemia = uni.dodajPlanet�(10, { 0,100.0 }, { 7,0 }, sf::Color::Cyan);	//Ziemia
	uni.dodajPlanet�(0.027, { 0,105.0 }, { 10,0 }, sf::Color::White);				//Ksi�yc
	uni.dodajPlanet�(0.1, { 0, 200 }, { 5,0 }, sf::Color(160, 103, 46));			//Mars
}

void scen_2S�o�ca1Planeta(Uniwersum & uni)			//xDDDDDDDDDDDD
{
	//Planety domy�lne
	uni.dodajPlanet�(1000, 5, { -50, 0 }, { 0,0 }, sf::Color::Yellow, true);
	uni.dodajPlanet�(1000, 5, { 50, 0 }, { 0,0 }, sf::Color::Yellow, true);
	uni.dodajPlanet�(10, { 0, 0 }, { 7.35, 9 }, sf::Color::Red, false);
	uni.dodajPlanet�(0.1, { 0, 100 }, { 9, 0 }, sf::Color::Cyan, false);
	uni.dodajPlanet�(0.1, { -50, 8 }, { 24, 0 }, sf::Color::White, false);

}

void scen_Uk�Binarny(Uniwersum & uni)
{
	//Planety domy�lne
	uni.dodajPlanet�(1000000, { -150, 0 }, { 0,100 }, sf::Color::Cyan);
	uni.dodajPlanet�(1000000, { 150, 0 }, { 0,-100 }, sf::Color::Yellow);
	uni.dodajPlanet�(1000, { 3000, 0 }, { 0, -57 }, sf::Color::White);
	uni.dodajPlanet�(1, { 3020, 0 }, { 0, -74 }, sf::Color::Green);
	uni.dodajPlanet�(0.1, { 800, 0 }, { 0, -120 }, sf::Color::Red);

	uni.zoomujWidoku(10.0f);

}


void scen_du�oPlanetekR�wnomiernie(Uniwersum & uni)
{
	float masaS�o�ca = 1000;
	uni.dodajPlanet�(masaS�o�ca, { 0,0 }, { 0,0 }, sf::Color::Cyan, true);				//S�o�ce

	for (int i = 0; i < 20; i++)
	{
		float R = 12.0f + i * 4.0f;
		float v = sqrt(static_cast<float>(G_Sta�aGrawitacji) * masaS�o�ca / R);
		uni.dodajPlanet�(masaS�o�ca / 1e4, { 0, R }, { v, 0 }, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)), false);
	}
}

void scen_du�oPlanetek(Uniwersum & uni)
{
	float masaS�o�ca = 1000;
	uni.dodajPlanet�(masaS�o�ca, { 0,0 }, { 0,0 }, sf::Color::Cyan, true);				//S�o�ce

	for (int i = 0; i < 10; i++)
	{
		float R = 12.0f + i * i * 0.7f;
		float v = sqrt(static_cast<float>(G_Sta�aGrawitacji) * masaS�o�ca / R);
		uni.dodajPlanet�(masaS�o�ca / 1e6, { 0, R }, { v, 0 }, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)), false);
	}
}

void wylosujScenariusz(Uniwersum & uni)
{
	switch (rand(1, 5))
	{
	case 1:
		scen_PlanetyWewn�trzne(uni); break;
	case 2:
		scen_2S�o�ca1Planeta(uni); break;
	case 3:
		scen_Uk�Binarny(uni); break;
	case 4:
		scen_du�oPlanetekR�wnomiernie(uni); break;
	case 5:
		scen_du�oPlanetek(uni); break;
	default:
		scen_PlanetyWewn�trzne(uni); break;
	}
}