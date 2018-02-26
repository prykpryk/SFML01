#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Obiekt.h"
#include "PlanetaSta³a.h"
#include "ZmienneGlob.h"

#include "f_Ró¿ne.h"
#include "f_UI.h"

int rand(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 mersenne{ rd() };
	static const double fraction = 1.0 / (static_cast<double>(mersenne.max()) + 1.0);
	return min + static_cast<int>((max - min + 1) * (mersenne() * fraction));
}


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
				skalujWidokOkna(window, 1.2f);
				break;
			case Keyboard::Add:
				skalujWidokOkna(window, 0.8f);
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
			zoomScroll(&window, &event);
			break;
		default:
			break;
		}
	}
}


int odl2(Vector2i a, Vector2i b)
{
	Vector2i c = b - a;
	return sqrt(c.x*c.x + c.y*c.y);
}