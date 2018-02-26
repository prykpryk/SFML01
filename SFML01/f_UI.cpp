#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Obiekt.h"
#include "PlanetaSta�a.h"
#include "ZmienneGlob.h"
#include "f_R�ne.h"
#include "f_UI.h"

void handleEvents(RenderWindow &window, std::vector<Obiekt*> &tablicaObiekt�w)
{
	Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:		//Zamknij okno
			window.close(); break;
		case Event::Resized:	//Zmie� rozmiar okna
			G_WINWIDTH = event.size.width;
			G_WINHEIGHT = event.size.height;
			std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
			break;
		case Event::KeyPressed:
			switch (event.key.code)
			{
			case Keyboard::Up:
				przesu�WidokOkna(window, Vector2f(0, -10));
				break;
			case Keyboard::Down:
				przesu�WidokOkna(window, Vector2f(0, 10));
				break;
			case Keyboard::Left:
				przesu�WidokOkna(window, Vector2f(-10, 0));
				break;
			case Keyboard::Right:
				przesu�WidokOkna(window, Vector2f(10, 0));
				break;
			case Keyboard::Subtract:
				wybierzRozmiar(-1);
				break;
			case Keyboard::Add:
				wybierzRozmiar(1);
				break;
			case Keyboard::Comma:
				wybierzPr�dko��Symulacji(0.5);
				break;
			case Keyboard::Period:
				wybierzPr�dko��Symulacji(2.0);
				break;
			default:
				break;
			}
			break;
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button)
			{
			case Mouse::Left:
				narysujObiekt(&window, &tablicaObiekt�w);
				break;
			case Mouse::Right:
				usunObiektKursor(&window, &tablicaObiekt�w);
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


void generujObiekt(std::vector<Obiekt*> *tablicaObiekt�w)
{
	mu_tObiekt�w.lock();
	auto ptr = new Obiekt(tablicaObiekt�w, 1, { 0,0 }, { 0,0 }, 10, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiekt�w.unlock();

	if (DEBUG)
		std::cout << "Stworzono obiekt w adresie: " << ptr << "\n";
}

void narysujObiekt(RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w)
{
	Vector2f pocz�tek = window->mapPixelToCoords(Mouse::getPosition(*window));
	while (Mouse::isButtonPressed(Mouse::Left))
	{
	}
	Vector2f koniec = window->mapPixelToCoords(Mouse::getPosition(*window));
	Vector2f pr�dko�� = { (koniec.x - pocz�tek.x)*1.0f, (koniec.y - pocz�tek.y)*1.0f };
	
	double masa = pow(10.0, G_WybranaWielkos�);
	double �rednica = cbrt(masa);

	mu_tObiekt�w.lock();
	auto ptr = new Obiekt(tablicaObiekt�w, �rednica,
		static_cast<Vector2d>(pocz�tek) / G_PIKSELI_NA_METR, static_cast<Vector2d>(pr�dko��) / G_PIKSELI_NA_METR,
		masa, Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	mu_tObiekt�w.unlock();

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << " o predkosci " << pr�dko��.x << " " << pr�dko��.y << "\n";
}

void usunPierwszyObiekt(std::vector<Obiekt*> *tablicaObiekt�w)
{
	auto it = tablicaObiekt�w->begin();
	//std::cout << *it << "\n";
	if (it != tablicaObiekt�w->end())
	{
		if (DEBUG)
			std::cout << "Usunieto obiekt w adresie: " << *it << "\n";
		delete *it;
	}
	else
		std::cout << "Nie ma nic do usuniecia.\n";
}

void usunObiektKursor(RenderWindow *window, std::vector<Obiekt*> *tablicaObiekt�w)
{
	Vector2i pocz�tek = Mouse::getPosition(*window);

	Obiekt *wskazany = nullptr;

	for (Obiekt *obiekt : *tablicaObiekt�w)
	{
		Vector2i pozycjaNaEkranie = window->mapCoordsToPixel(obiekt->m_circle.getPosition());
		int odle = odl2(pocz�tek, pozycjaNaEkranie);

		if (odle <= obiekt->m_circle.getRadius())
		{
			wskazany = obiekt;
			break;
		}
	}

	delete wskazany;

}

void przesu�WidokOkna(RenderWindow &window, Vector2f a)
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
	Vector2f �rodek = view.getCenter();

	float zmianaSkali = - G_ZOOM_SENS * event.mouseWheelScroll.x;

	if (zmianaSkali > G_ZOOM_MAXSPEED)
		zmianaSkali = G_ZOOM_MAXSPEED;

	if (event.mouseWheelScroll.delta < 0)
		zmianaSkali = -zmianaSkali;

	�rodek -= zmianaSkali * (kursorCoord - �rodek);

	view.zoom(1.0f + zmianaSkali);
	view.setCenter(�rodek);
	window.setView(view);

}

void wybierzRozmiar(int i)
{
	G_WybranaWielkos� += i;
	std::cout << "Wybrano mas� planety " << pow(10.0, G_WybranaWielkos�) << " kg.\n";
}

void wybierzPr�dko��Symulacji(double i)
{
	G_Pr�dko��Symulacji *= i;
	std::cout << "Wybrano pr�dko�� symulacji " << G_Pr�dko��Symulacji << "x.\n";
}

void instrukcja()
{
	std::cout << "Klawiszologia:\n"
		"LPM\t\t- stworzenie planety\n"
		"LPM przeci�g.\t- stworzenie planety z pr�dko�ci�\n"
		"PPM\t\t- usuni�cie planety\n"
		"Strza�ki\t- przesuwanie mapy\n"
		"Scroll\t\t- zoom i przesuwanie mapy\n"
		"+ i -\t\t- zmiana rozmiaru tworzonej planety\n"
		", i .\t\t- zmiana szybko�ci symualcji\n\n";
	wybierzRozmiar(0);				//Tylko dla wy�wietlenia pocz�tkowej warto�ci
	wybierzPr�dko��Symulacji(1);	//Tylko dla wy�wietlenia pocz�tkowej warto�ci
}