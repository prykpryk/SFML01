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

int rand(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 mersenne{ rd() };
	static const double fraction = 1.0 / (static_cast<double>(mersenne.max()) + 1.0);
	return min + static_cast<int>((max - min + 1) * (mersenne() * fraction));
}


class Obiekt
{
protected:

	sf::Vector2f m_vel;
	sf::Vector2f m_F;
	float m_R;
	std::vector<Obiekt*> *m_tablicaObiektów;


public:
	sf::CircleShape m_circle;
	float m_masa;
	sf::Vector2f m_pos;

	Obiekt(std::vector<Obiekt*> *tablicaObiektów, float r, sf::Vector2f pos = { 0, 0 }, sf::Vector2f vel = { 0,0 }, float masa = 10, sf::Color color = sf::Color::White)
		: m_circle{ r / 2 * G_PIKSELI_NA_METR,16 }, m_pos{ pos }, m_vel{ vel }, m_tablicaObiektów{ tablicaObiektów },
		m_R{ r }, m_masa{ masa }
	{
		m_circle.setPosition(m_pos*G_PIKSELI_NA_METR);
		m_circle.setOrigin(r / 2 * G_PIKSELI_NA_METR, r / 2 * G_PIKSELI_NA_METR);
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

	float odległość(Obiekt *planeta)
	{
		return sqrt(pow(m_pos.x - planeta->m_pos.x, 2) + pow(m_pos.y - planeta->m_pos.y, 2));
	}

	sf::Vector2f obliczSiłyGrawitacji()
	{
		sf::Vector2f sumaSił = { 0,0 };
		for (auto planeta : *m_tablicaObiektów)
		{
			if (planeta != this)
			{
				float wypadkowa = (m_masa * planeta->m_masa) / pow(odległość(planeta), 2);
				float alfa = atan2(planeta->m_pos.x - m_pos.x, planeta->m_pos.y - m_pos.y);
				sumaSił.x += wypadkowa * sin(alfa);
				sumaSił.y += wypadkowa * cos(alfa);
			}
		}
		return sumaSił * 5.0f;
	}

	virtual void obliczPozycję(sf::Time &czas)
	{
		m_F = obliczSiłyGrawitacji();

		sf::Vector2f przysp = m_F / m_masa;

		m_pos.x += 0.5f * m_vel.x * czas.asSeconds();
		m_pos.y += 0.5f * m_vel.y * czas.asSeconds();

		m_vel += przysp * czas.asSeconds();

		m_pos.x += 0.5f * m_vel.x * czas.asSeconds();
		m_pos.y += 0.5f * m_vel.y * czas.asSeconds();

		m_circle.setPosition(m_pos.x*G_PIKSELI_NA_METR, m_pos.y*G_PIKSELI_NA_METR);

	}
};

class PlanetaStała : public Obiekt
{
public:
	PlanetaStała(std::vector<Obiekt*> *tablicaObiektów, float r, sf::Vector2f pos, sf::Vector2f vel = { 0,0 }, float masa = 10, sf::Color color = sf::Color::White)
		: Obiekt{tablicaObiektów, r, pos, vel, masa, color }
	{
	};
	virtual void obliczPozycję(sf::Time &czas)
	{};
};

void fRysowania(sf::RenderWindow &window, std::vector<Obiekt*> &tablicaObiektów)
{
	window.clear(sf::Color::Black);

	for (Obiekt *obiekt : tablicaObiektów)
	{
		window.draw(obiekt->m_circle);
	}

	window.display();
}



void generujObiekt(std::vector<Obiekt*> *tablicaObiektów)
{
	klatka.lock();
	auto ptr = new Obiekt (tablicaObiektów, 1, { 0,0 }, { 0,0 }, 10, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	klatka.unlock();

	if (DEBUG)
		std::cout << "Stworzono obiekt w adresie: " << ptr << "\n";
}

void narysujObiekt(sf::RenderWindow *window, std::vector<Obiekt*> *tablicaObiektów)
{
	sf::Vector2i początek = sf::Mouse::getPosition(*window);
	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{}
	sf::Vector2i koniec = sf::Mouse::getPosition(*window);
	sf::Vector2f prędkość = { (koniec.x - początek.x)*0.1f,(koniec.y - początek.y)*0.1f };


	klatka.lock();
	auto ptr = new Obiekt(tablicaObiektów, 1, { początek.x/G_PIKSELI_NA_METR, początek.y/G_PIKSELI_NA_METR }, prędkość, 10, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));
	klatka.unlock();

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << " o predkosci " << prędkość.x << " " << prędkość.y <<"\n";
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
			obiekt->obliczPozycję(czas);
		}
		klatka.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); //TODO coś kraszuje bez tego, ten wątek chce odświeżyć niekompletny obiekt?
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
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
				narysujObiekt(&window, &tablicaObiektów);
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

	//Obiekt planeta(&tablicaObiektów, 1, { 20,20 }, { 0,0 }, 10, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));

	PlanetaStała *słońce = new PlanetaStała (&tablicaObiektów, 4, { 20,16 }, { 0,0 }, 200, sf::Color::Yellow);

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