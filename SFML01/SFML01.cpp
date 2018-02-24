// SFML01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "iostream"
#include <math.h>
#include <vector>
#include <thread>

const	float		G_PIKSELI_NA_METR{ 20.0f };
const	float		G_ODBIJALNOSC{ 0.6f };
const	float		G_PRZYSP_ZIEMSKIE{ 9.80665f };
		int			G_WINWIDTH{ 800 };
		int			G_WINHEIGHT{ 600 };

class Obiekt
{
private:
	float m_vel_X;	//prędkość w m/s
	float m_vel_Y;
	float m_posm_X;	//pozycja w metrach według układu X-Y
	float m_posm_Y;

	void sprawdzKolizje()
	{	
		//TODO::Uwzględnić zmianę rozmiaru okna

		sf::Vector2f pozycja = m_circle.getPosition();

		//if ((pozycja.y + m_circle.getRadius() > G_WINHEIGHT && m_vel_Y > 0)
		//	|| (pozycja.y + m_circle.getRadius() < 0 && m_vel_Y < 0))
		//{
		//	m_vel_Y = -m_vel_Y * G_ODBIJALNOSC;
		//	//std::cout << "Kolizja pionowa";
		//}
		//
		//if ((pozycja.x + m_circle.getRadius() > G_WINWIDTH && m_vel_X > 0)
		//	|| (pozycja.x - m_circle.getRadius() < 0 && m_vel_X < 0))
		//{
		//	m_vel_X = -m_vel_X * G_ODBIJALNOSC;
		//	//std::cout << "Kolizja pozioma";
		//}

		if (pozycja.y + m_circle.getRadius() > G_WINHEIGHT && m_vel_Y > 0)	//kolizja z dołu
			m_vel_Y = -abs(m_vel_Y) * G_ODBIJALNOSC;
		if (pozycja.y - m_circle.getRadius() < 0 && m_vel_Y < 0)	//kolizja z gory
			m_vel_Y = abs(m_vel_Y) * G_ODBIJALNOSC;
		if (pozycja.x + m_circle.getRadius() > G_WINWIDTH && m_vel_X > 0)	//kolizja z prawej
			m_vel_X = -abs(m_vel_X) * G_ODBIJALNOSC;
		if (pozycja.x - m_circle.getRadius() < 0 && m_vel_X < 0)	//kolizja z lewej
			m_vel_X = abs(m_vel_X) * G_ODBIJALNOSC;
	}

	void nadajPrzyspieszenie(sf::Time &czas)
	{
		float delta = 10*czas.asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			m_vel_Y -= delta;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			m_vel_Y += delta;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			m_vel_X -= delta;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			m_vel_X += delta;

	}

	void nadajGrawitacje(sf::Time &czas)
	{
		m_vel_Y += (G_PRZYSP_ZIEMSKIE * czas.asSeconds());		//grawitacja
	}


public:
	sf::CircleShape m_circle;

	Obiekt(std::vector<Obiekt*> *tablicaObiektów, float średnica_m, float pos_X = 0, float pos_Y = 0, float vel_X = 0, float vel_Y = 0, sf::Color color = sf::Color::White)
		: m_circle{ średnica_m / 2 * G_PIKSELI_NA_METR,16 }, m_posm_X{ pos_X }, m_posm_Y{ pos_Y }, m_vel_X{ vel_X }, m_vel_Y{ vel_Y }
	{
		m_circle.setOrigin(średnica_m / 2 * G_PIKSELI_NA_METR, średnica_m / 2 * G_PIKSELI_NA_METR);
		m_circle.setFillColor(color);
		tablicaObiektów->push_back(this);
	};

	void dSprawdzPozycje()
	{
			std::cout << m_circle.getPosition().x << " " << m_circle.getPosition().y << "\n";
	}


	void odswiezPilke(sf::Time &czas)
	{
		nadajGrawitacje(czas);
		nadajPrzyspieszenie(czas);

		m_posm_X += m_vel_X * czas.asSeconds(); //TODO powinna być średnia ze starej i nowej prędkości 
		m_posm_Y += m_vel_Y * czas.asSeconds(); //TODO powinna być średnia ze starej i nowej prędkości 

		m_circle.setPosition(m_posm_X*G_PIKSELI_NA_METR, m_posm_Y*G_PIKSELI_NA_METR);

		sprawdzKolizje();
	}
};

//std::vector<Obiekt> G_tablicaObiektów;		//No ale kurwa żeby globala używać jak jakiś smieć? 

void fRysowania(sf::RenderWindow &window, std::vector<Obiekt*> &tablicaObiektów)
{
	window.clear(sf::Color::Black);

	for (auto obiekt : tablicaObiektów)
	{
		window.draw(obiekt->m_circle);
	}

	window.display();
};

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(G_WINWIDTH, G_WINHEIGHT), "Moje okienko xDD",sf::Style::Default,settings);

	sf::Clock clock;

	std::vector<Obiekt*> tablicaObiektów;

	Obiekt pilka(&tablicaObiektów, 1, 0, 0, 0, 0, sf::Color::Red);
	Obiekt pilka2(&tablicaObiektów, 2, 1, 0, 0, 0, sf::Color::Yellow);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close(); break;
			case sf::Event::Resized:
				G_WINWIDTH = event.size.width;
				G_WINHEIGHT = event.size.height;
				std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::D)
				{
					pilka.dSprawdzPozycje();
				};
				break;
			default: 
				break;
			}
		}

		sf::Time czas = clock.restart();

		fRysowania(window, tablicaObiektów);

		for (auto obiekt : tablicaObiektów)
		{
			obiekt->odswiezPilke(czas);
		}

	}

	return 0;
}