#pragma once

#include "stdafx.h"
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"
#include "ZmienneGlob.h"

class Planeta : public CircleShape
{
protected:

	Vector2d m_vel;
	Vector2d m_F;
	double m_R;
	std::vector<Planeta*> *m_tablicaObiektów;
	Color m_color;

public:
	std::vector<Vertex> m_œlad;
	double m_masa;
	Vector2d m_pos;

	Planeta(
		std::vector<Planeta*> *tablicaObiektów, 
		const double r, 
		const Vector2d pos = { 0, 0 }, 
		const Vector2d vel = { 0,0 }, 
		const double masa = 10, 
		const Color color = Color::White)
		: CircleShape{ r / 2 * G_PIKSELI_NA_METR,16 }, m_pos{ pos }, m_vel{ vel }, m_tablicaObiektów{ tablicaObiektów },
		m_R{ r }, m_masa{ masa }, m_color{ color }
	{
		setPosition(static_cast<Vector2f>(m_pos*G_PIKSELI_NA_METR));
		setOrigin(r / 2 * G_PIKSELI_NA_METR, r / 2 * G_PIKSELI_NA_METR);
		setFillColor(m_color);
		//m_œlad.setPrimitiveType(LinesStrip);

		odœwie¿Œlad();
		tablicaŒladów.push_back(&m_œlad);
	};

	~Planeta()
	{
		//Usuniêcie obiektu z tablicy obiektów

		//mu_tObiektów.lock();
		for (auto it = m_tablicaObiektów->begin(); it != m_tablicaObiektów->end(); )
		{
			if (*it == this)
				it = m_tablicaObiektów->erase(it);
			else
				it++;
		}
		//mu_tObiektów.lock();

		//mu_tŒladów.lock();
		for (auto it = tablicaŒladów.begin(); it != tablicaŒladów.end(); )
		{
			if (*it == &m_œlad)
				it = tablicaŒladów.erase(it);
			else
				it++;
		}
		//mu_tŒladów.unlock();
	}

	double odleg³oœæ(const Planeta *planeta)
	{
		return sqrt(pow(m_pos.x - planeta->m_pos.x, 2) + pow(m_pos.y - planeta->m_pos.y, 2));
	}

	Vector2d obliczSi³yGrawitacji()
	{
		Vector2d sumaSi³ = { 0,0 };
		for (auto planeta : *m_tablicaObiektów)
		{
			if (planeta != this)
			{
				double wypadkowa = G_Sta³aGrawitacji * (m_masa * planeta->m_masa) / pow(odleg³oœæ(planeta), 2);
				double alfa = atan2(planeta->m_pos.x - m_pos.x, planeta->m_pos.y - m_pos.y);
				sumaSi³.x += wypadkowa * sin(alfa);
				sumaSi³.y += wypadkowa * cos(alfa);
			}
		}
		return sumaSi³;
	}

	virtual void obliczPozycjê(const double &czas)		//Musi byæ referencja, inaczej crash???
	{
		m_F = obliczSi³yGrawitacji();


		Vector2d przysp = m_F / m_masa;			//Ca³kowanie przybli¿one trapezami, dok³adne przy sta³ym przyspieszeniu

		m_pos.x += 0.5f * m_vel.x * czas;
		m_pos.y += 0.5f * m_vel.y * czas;

		m_vel += przysp * czas;

		m_pos.x += 0.5f * m_vel.x * czas;
		m_pos.y += 0.5f * m_vel.y * czas;

		setPosition(m_pos.x*G_PIKSELI_NA_METR, m_pos.y*G_PIKSELI_NA_METR);
	}

	virtual void odœwie¿Œlad()
	{
		mu_tŒladów.lock();
		m_œlad.push_back(Vertex(static_cast<Vector2f>(m_pos*G_PIKSELI_NA_METR), m_color));

		if (m_œlad.size() > G_D£UGOŒÆ_ŒLADU)
			m_œlad.erase(m_œlad.begin());

		mu_tŒladów.unlock();
	}
};