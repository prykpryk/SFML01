#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Obiekt.h"
#include "PlanetaSta�a.h"
#include "ZmienneGlob.h"

#include "f_R�ne.h"
#include "f_UI.h"

int rand(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 mersenne{ rd() };
	static const double fraction = 1.0 / (static_cast<double>(mersenne.max()) + 1.0);
	return min + static_cast<int>((max - min + 1) * (mersenne() * fraction));
}

void handleEvents(sf::RenderWindow &window, std::vector<Obiekt*> &tablicaObiekt�w)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:		//Zamknij okno
			window.close(); break;
		case sf::Event::Resized:	//Zmie� rozmiar okna
			G_WINWIDTH = event.size.width;
			G_WINHEIGHT = event.size.height;
			std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::S:
				generujObiekt(&tablicaObiekt�w);
				break;
			case sf::Keyboard::Delete:
				usunPierwszyObiekt(&tablicaObiekt�w);
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				narysujObiekt(&window, &tablicaObiekt�w);
				break;
			case sf::Mouse::Right:
				usunObiektKursor(&window, &tablicaObiekt�w);
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