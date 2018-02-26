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

void handleEvents(sf::RenderWindow &window, std::vector<Obiekt*> &tablicaObiektów)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:		//Zamknij okno
			window.close(); break;
		case sf::Event::Resized:	//Zmieñ rozmiar okna
			G_WINWIDTH = event.size.width;
			G_WINHEIGHT = event.size.height;
			std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				przesuñWidokOkna(window, sf::Vector2f(0, -10));
				break;
			case sf::Keyboard::Down:
				przesuñWidokOkna(window, sf::Vector2f(0, 10));
				break;
			case sf::Keyboard::Left:
				przesuñWidokOkna(window, sf::Vector2f(-10, 0));
				break;
			case sf::Keyboard::Right:
				przesuñWidokOkna(window, sf::Vector2f(10, 0));
				break;
			case sf::Keyboard::Add:
				skalujWidokOkna(window, 1.2f);
				break;
			case sf::Keyboard::Subtract:
				skalujWidokOkna(window, 0.8f);
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				narysujObiekt(&window, &tablicaObiektów);
				break;
			case sf::Mouse::Right:
				usunObiektKursor(&window, &tablicaObiektów);
				break;
			}
		default:
			break;
		}
	}
}



int odl2(sf::Vector2i a, sf::Vector2i b)
{
	sf::Vector2i c = b - a;
	return sqrt(c.x*c.x + c.y*c.y);
}