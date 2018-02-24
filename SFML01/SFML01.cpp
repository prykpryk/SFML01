// SFML01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "iostream"

float	G_PIKSELI_NA_METR{ 20.0f };
float	G_ODBIJALNOSC{ 0.9f };
float	G_PRZYSP_ZIEMSKIE{ 9.80665f };
int		G_WINWIDTH{ 800 };
int		G_WINHEIGHT{ 600 };

class Piłka
{
private:
	float m_vel_X = 8;	//prędkość w m/s
	float m_vel_Y = 0;
	float m_posm_X = 0;	//pozycja w metrach według układu X-Y
	float m_posm_Y = 0;

	void sprawdzKolizje()
	{
		sf::Vector2f pozycja = m_circle.getPosition();

		if ((pozycja.y + m_circle.getRadius() > G_WINHEIGHT && m_vel_Y > 0)
			|| (pozycja.y + m_circle.getRadius() < 0 && m_vel_Y < 0))
		{
			m_vel_Y = -m_vel_Y * G_ODBIJALNOSC;
			std::cout << "Kolizja pionowa";
		}

		if ((pozycja.x + m_circle.getRadius() > G_WINWIDTH && m_vel_X > 0)
			|| (pozycja.x - m_circle.getRadius() < 0 && m_vel_X < 0))
		{
			m_vel_X = -m_vel_X * G_ODBIJALNOSC;
			std::cout << "Kolizja pozioma";
		}
	}

public:
	sf::CircleShape m_circle;

	Piłka(float średnica_m) : m_circle{ średnica_m/2*G_PIKSELI_NA_METR,16 }
	{
		m_circle.setOrigin(średnica_m / 2 * G_PIKSELI_NA_METR, średnica_m / 2 * G_PIKSELI_NA_METR);
	};

	void odswiezPilke(sf::Time &czas)
	{
		m_vel_Y += (G_PRZYSP_ZIEMSKIE * czas.asSeconds());		//grawitacja

		m_posm_X += m_vel_X * czas.asSeconds(); //TODO powinna być średnia ze starej i nowej prędkości 
		m_posm_Y += m_vel_Y * czas.asSeconds(); //TODO powinna być średnia ze starej i nowej prędkości 

		m_circle.setPosition(m_posm_X*G_PIKSELI_NA_METR, m_posm_Y*G_PIKSELI_NA_METR);
		sprawdzKolizje();
	}
};

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(G_WINWIDTH, G_WINHEIGHT), "Moje okienko xDD",sf::Style::Default,settings);

	sf::Clock clock;

	//sf::CircleShape pilka(10,16);
	//pilka.setFillColor(sf::Color::White);
	//pilka.setOrigin(10, 10);
	//pilka.setPosition(0, 10);

	Piłka pilka(1);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time czas = clock.restart();

		pilka.odswiezPilke(czas);

		window.clear(sf::Color::Black);

		window.draw(pilka.m_circle);

		window.display();
	}

	return 0;
}