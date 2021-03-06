// SFML01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "iostream"
#include <math.h>
#include <vector>
#include <thread>
#include <random>
#include <mutex>
#include <chrono>

#define DEBUG true

const	float		G_PIKSELI_NA_METR{ 20.0f };
const	float		G_ODBIJALNOSC{ 0.6f };
const	float		G_PRZYSP_ZIEMSKIE{ 9.80665f };
		int			G_WINWIDTH{ 800 };
		int			G_WINHEIGHT{ 600 };
		static	std::mutex	klatka;

int randi(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 mersenne{ rd() };
	// Note: Due to a bug in the Code::Blocks compiler, if using Code::Blocks on Windows, delete the two lines above and uncomment this line:
	// static std::mt19937 mersenne(static_cast<unsigned int>(time(0))); // initialize our mersenne twister with a random seed
	static const double fraction = 1.0 / (static_cast<double>(mersenne.max()) + 1.0);
	return min + static_cast<int>((max - min + 1) * (mersenne() * fraction));
}


class Obiekt
{
private:

	sf::Vector2f m_vel;
	sf::Vector2f m_pos;
	std::vector<Obiekt*> *m_tablicaObiektów;

	void sprawdzKolizje()
	{	
		sf::Vector2f pozycja = m_circle.getPosition();

		if (pozycja.y + m_circle.getRadius() > G_WINHEIGHT && m_vel.y > 0)	//kolizja z dołu
			m_vel.y = -abs(m_vel.y) * G_ODBIJALNOSC;
		if (pozycja.y - m_circle.getRadius() < 0 && m_vel.y < 0)	//kolizja z gory
			m_vel.y = abs(m_vel.y) * G_ODBIJALNOSC;
		if (pozycja.x + m_circle.getRadius() > G_WINWIDTH && m_vel.x > 0)	//kolizja z prawej
			m_vel.x = -abs(m_vel.x) * G_ODBIJALNOSC;
		if (pozycja.x - m_circle.getRadius() < 0 && m_vel.x < 0)	//kolizja z lewej
			m_vel.x = abs(m_vel.x) * G_ODBIJALNOSC;
	}

	void nadajPrzyspieszenie(sf::Time &czas)
	{
		float delta = 10*czas.asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			m_vel.y -= delta;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			m_vel.y += delta;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			m_vel.x -= delta;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			m_vel.x += delta;

	}

	void nadajGrawitacje(sf::Time &czas)
	{
		m_vel.y += (G_PRZYSP_ZIEMSKIE * czas.asSeconds());		//grawitacja
	}


public:
	sf::CircleShape m_circle;

	Obiekt(std::vector<Obiekt*> *tablicaObiektów, float średnica_m, sf::Vector2f pos = { 0, 0 }, sf::Vector2f vel = { 0,0 }, sf::Color color = sf::Color::White)
		: m_circle{ średnica_m / 2 * G_PIKSELI_NA_METR,16 }, m_pos{ pos }, m_vel{ vel }, m_tablicaObiektów{ tablicaObiektów }
	{
		m_circle.setOrigin(średnica_m / 2 * G_PIKSELI_NA_METR, średnica_m / 2 * G_PIKSELI_NA_METR);
		m_circle.setFillColor(color);
		tablicaObiektów->push_back(this);
	};

	~Obiekt()
	{
		//Usunięcie obiektu z tablicy obiektów
		for (auto it = m_tablicaObiektów->begin(); it != m_tablicaObiektów->end(); )
		{
			if ( *it == this )
				it = m_tablicaObiektów->erase(it);
			else
				it++;
		}
	}

	void dSprawdzPozycje()
	{
			std::cout << m_circle.getPosition().x << " " << m_circle.getPosition().y << "\n";
	}


	void odswiezPilke(sf::Time &czas)
	{
		nadajGrawitacje(czas);
		nadajPrzyspieszenie(czas);

		m_pos.x += m_vel.x * czas.asSeconds(); //TODO powinna być średnia ze starej i nowej prędkości 
		m_pos.y += m_vel.y * czas.asSeconds(); //TODO powinna być średnia ze starej i nowej prędkości 

		m_circle.setPosition(m_pos.x*G_PIKSELI_NA_METR, m_pos.y*G_PIKSELI_NA_METR);

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
}

void generujObiekt(std::vector<Obiekt*> *tablicaObiektów)
{
	klatka.lock();
	auto ptr = new Obiekt (tablicaObiektów, 1, { 0,0 }, { 0,0 }, sf::Color(randi(0, 255), randi(0, 255), randi(0, 255)));
	klatka.unlock();

	if (DEBUG)
		std::cout << "Stworzono obiekt w adresie: " << ptr << "\n";

}

void usunObiekt(std::vector<Obiekt*> *tablicaObiektów)
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

void t_Fizyka(const sf::RenderWindow *window, const std::vector<Obiekt*> *tablicaObiektów)
{
	sf::Clock clock;
	while (window->isOpen())
	{		
		klatka.lock();
		sf::Time czas = clock.restart();
		for (auto obiekt : *tablicaObiektów)
		{
			obiekt->odswiezPilke(czas);
		}
		klatka.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); //TODO coś kraszuje bez tego, ten wątek chce odświeżyć niekompletny obiekt?
	}
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
		case sf::Event::Resized:	//Zmień rozmiar okna
			G_WINWIDTH = event.size.width;
			G_WINHEIGHT = event.size.height;
			std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::S:
				generujObiekt(&tablicaObiektów);
				break;
			case sf::Keyboard::Delete:
				usunObiekt(&tablicaObiektów);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(G_WINWIDTH, G_WINHEIGHT), "Moje okienko xDD",sf::Style::Default,settings);

	std::vector<Obiekt*> tablicaObiektów;

	//Wątek fizyki
	std::thread t1(t_Fizyka, &window, &tablicaObiektów);

	while (window.isOpen())
	{
		handleEvents(window, tablicaObiektów);
		fRysowania(window, tablicaObiektów);

	}
	
	t1.join();
	return 0;
}