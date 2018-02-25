#pragma once

#include "stdafx.h"
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"

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
		//Usuniêcie obiektu z tablicy obiektów
		for (auto it = m_tablicaObiektów->begin(); it != m_tablicaObiektów->end(); )
		{
			if (*it == this)
				it = m_tablicaObiektów->erase(it);
			else
				it++;
		}
	}

	float odleg³oœæ(Obiekt *planeta)
	{
		return sqrt(pow(m_pos.x - planeta->m_pos.x, 2) + pow(m_pos.y - planeta->m_pos.y, 2));
	}

	sf::Vector2f obliczSi³yGrawitacji()
	{
		sf::Vector2f sumaSi³ = { 0,0 };
		for (auto planeta : *m_tablicaObiektów)
		{
			if (planeta != this)
			{
				float wypadkowa = 5.0f * (m_masa * planeta->m_masa) / pow(odleg³oœæ(planeta), 2);
				float alfa = atan2(planeta->m_pos.x - m_pos.x, planeta->m_pos.y - m_pos.y);
				sumaSi³.x += wypadkowa * sin(alfa);
				sumaSi³.y += wypadkowa * cos(alfa);
			}
		}
		return sumaSi³;
	}

	virtual void obliczPozycjê(sf::Time &czas)
	{
		m_F = obliczSi³yGrawitacji();


		sf::Vector2f przysp = m_F / m_masa;

		m_pos.x += 0.5f * m_vel.x * czas.asSeconds();
		m_pos.y += 0.5f * m_vel.y * czas.asSeconds();

		m_vel += przysp * czas.asSeconds();

		m_pos.x += 0.5f * m_vel.x * czas.asSeconds();
		m_pos.y += 0.5f * m_vel.y * czas.asSeconds();

		m_circle.setPosition(m_pos.x*G_PIKSELI_NA_METR, m_pos.y*G_PIKSELI_NA_METR);

	}
};