#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Obiekt.h"
#include "PlanetaSta³a.h"
#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"
#include "f_UI.h"

void handleEvents(RenderWindow &window, std::vector<Obiekt*> &tablicaObiektów)
{
	Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:		//Zamknij okno
			window.close(); break;
		case Event::Resized:	//Zmieñ rozmiar okna
			G_WINWIDTH = event.size.width;
			G_WINHEIGHT = event.size.height;
			std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
			break;
		case Event::KeyPressed:
			switch (event.key.code)
			{
			case Keyboard::Up:
				przesuñWidokOkna(window, Vector2f(0, -10));
				break;
			case Keyboard::Down:
				przesuñWidokOkna(window, Vector2f(0, 10));
				break;
			case Keyboard::Left:
				przesuñWidokOkna(window, Vector2f(-10, 0));
				break;
			case Keyboard::Right:
				przesuñWidokOkna(window, Vector2f(10, 0));
				break;
			case Keyboard::Subtract:
				wybierzRozmiar(-1);
				break;
			case Keyboard::Add:
				wybierzRozmiar(1);
				break;
			case Keyboard::Comma:
				wybierzPrêdkoœæSymulacji(0.5);
				break;
			case Keyboard::Period:
				wybierzPrêdkoœæSymulacji(2.0);
				break;
			default:
				break;
			}
			break;
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button)
			{
			case Mouse::Left:
				narysujObiekt(&window, &tablicaObiektów);
				break;
			case Mouse::Right:
				usunObiektKursor(&window, &tablicaObiektów);
				break;
			}
			break;
		case Event::MouseWheelScrolled:
			zoomScroll(window, event);
			break;
		default:
			break;
		}
	}
}


void generujObiekt(std::vector<Obiekt*> *tablicaObiektów)
{
	mu_tObiektów.lock();
	auto ptr = new Obiekt(tablicaObiektów, 1, { 0,0 }, { 0,0 }, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiektów.unlock();

	if (DEBUG)
		std::cout << "Stworzono obiekt w adresie: " << ptr << "\n";
}

void narysujObiekt(RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów)
{
	Vector2f pocz¹tek = window->mapPixelToCoords(Mouse::getPosition(*window));
	while (Mouse::isButtonPressed(Mouse::Left))
	{
	}
	Vector2f koniec = window->mapPixelToCoords(Mouse::getPosition(*window));
	Vector2f prêdkoœæ = { (koniec.x - pocz¹tek.x)*1.0f, (koniec.y - pocz¹tek.y)*1.0f };
	
	double masa = pow(10.0, G_WybranaWielkosæ);
	double œrednica = cbrt(masa);

	mu_tObiektów.lock();
	auto ptr = new Obiekt(tablicaObiektów, œrednica,
		static_cast<Vector2d>(pocz¹tek) / G_PIKSELI_NA_METR, static_cast<Vector2d>(prêdkoœæ) / G_PIKSELI_NA_METR,
		masa, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiektów.unlock();

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << " o predkosci " << prêdkoœæ.x << " " << prêdkoœæ.y << "\n";
}

void usunPierwszyObiekt(std::vector<Obiekt*> *tablicaObiektów)
{
	auto it = tablicaObiektów->begin();
	//std::cout << *it << "\n";
	if (it != tablicaObiektów->end())
	{
		if (DEBUG)
			std::cout << "Usunieto obiekt w adresie: " << *it << "\n";
		delete *it;
	}
	else
		std::cout << "Nie ma nic do usuniecia.\n";
}

void usunObiektKursor(RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów)
{
	Vector2i pocz¹tek = Mouse::getPosition(*window);

	Obiekt *wskazany = nullptr;

	for (Obiekt *obiekt : *tablicaObiektów)
	{
		Vector2i pozycjaNaEkranie = window->mapCoordsToPixel(obiekt->m_circle.getPosition());
		int odle = odl2(pocz¹tek, pozycjaNaEkranie);

		if (odle <= obiekt->m_circle.getRadius())
		{
			wskazany = obiekt;
			break;
		}
	}

	delete wskazany;

}

void przesuñWidokOkna(RenderWindow &window, Vector2f a)
{
	View view = window.getView();
	view.move(a * view.getSize().x / 160.0f);
	window.setView(view);
}

void skalujWidokOkna(RenderWindow &window, float a)
{
	View view = window.getView();
	view.zoom(a);
	window.setView(view);
}

void zoomScroll(RenderWindow &window, Event &event)
{
	View view = window.getView();

	Vector2f kursorCoord = window.mapPixelToCoords(Mouse::getPosition(window));
	Vector2f œrodek = view.getCenter();

	float zmianaSkali = - G_ZOOM_SENS * event.mouseWheelScroll.x;

	if (zmianaSkali > G_ZOOM_MAXSPEED)
		zmianaSkali = G_ZOOM_MAXSPEED;

	if (event.mouseWheelScroll.delta < 0)
		zmianaSkali = -zmianaSkali;

	œrodek -= zmianaSkali * (kursorCoord - œrodek);

	view.zoom(1.0f + zmianaSkali);
	view.setCenter(œrodek);
	window.setView(view);

}

void wybierzRozmiar(int i)
{
	G_WybranaWielkosæ += i;
	std::cout << "Wybrano masê planety " << pow(10.0, G_WybranaWielkosæ) << " kg.\n";
}

void wybierzPrêdkoœæSymulacji(double i)
{
	G_PrêdkoœæSymulacji *= i;
	std::cout << "Wybrano prêdkoœæ symulacji " << G_PrêdkoœæSymulacji << "x.\n";
}

void instrukcja()
{
	std::cout << "Klawiszologia:\n"
		"LPM\t\t- stworzenie planety\n"
		"LPM przeci¹g.\t- stworzenie planety z prêdkoœci¹\n"
		"PPM\t\t- usuniêcie planety\n"
		"Strza³ki\t- przesuwanie mapy\n"
		"Scroll\t\t- zoom i przesuwanie mapy\n"
		"+ i -\t\t- zmiana rozmiaru tworzonej planety\n"
		", i .\t\t- zmiana szybkoœci symualcji\n\n";
	wybierzRozmiar(0);				//Tylko dla wyœwietlenia pocz¹tkowej wartoœci
	wybierzPrêdkoœæSymulacji(1);	//Tylko dla wyœwietlenia pocz¹tkowej wartoœci
}