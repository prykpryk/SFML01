#pragma once

#include "stdafx.h"
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"
#include "ZmienneGlob.h"

class Planeta : public CircleShape
{
protected:

	Vector2d m_vel;
	Vector2d m_F;
	double m_R;
	std::vector<Planeta*> *m_tablicaObiekt�w;
	Color m_color;

public:
	std::vector<Vertex> m_�lad;
	double m_masa;
	Vector2d m_pos;

	Planeta(
		std::vector<Planeta*> *tablicaObiekt�w, 
		const double r, 
		const Vector2d pos = { 0, 0 }, 
		const Vector2d vel = { 0,0 }, 
		const double masa = 10, 
		const Color color = Color::White)
		: CircleShape{ r / 2 * G_PIKSELI_NA_METR,16 }, m_pos{ pos }, m_vel{ vel }, m_tablicaObiekt�w{ tablicaObiekt�w },
		m_R{ r }, m_masa{ masa }, m_color{ color }
	{
		setPosition(static_cast<Vector2f>(m_pos*G_PIKSELI_NA_METR));
		setOrigin(r / 2 * G_PIKSELI_NA_METR, r / 2 * G_PIKSELI_NA_METR);
		setFillColor(m_color);
		//m_�lad.setPrimitiveType(LinesStrip);

		od�wie��lad();
		tablica�lad�w.push_back(&m_�lad);
	};

	~Planeta()
	{
		//Usuni�cie obiektu z tablicy obiekt�w

		//mu_tObiekt�w.lock();
		for (auto it = m_tablicaObiekt�w->begin(); it != m_tablicaObiekt�w->end(); )
		{
			if (*it == this)
				it = m_tablicaObiekt�w->erase(it);
			else
				it++;
		}
		//mu_tObiekt�w.lock();

		//mu_t�lad�w.lock();
		for (auto it = tablica�lad�w.begin(); it != tablica�lad�w.end(); )
		{
			if (*it == &m_�lad)
				it = tablica�lad�w.erase(it);
			else
				it++;
		}
		//mu_t�lad�w.unlock();
	}

	double odleg�o��(const Planeta *planeta)
	{
		return sqrt(pow(m_pos.x - planeta->m_pos.x, 2) + pow(m_pos.y - planeta->m_pos.y, 2));
	}

	Vector2d obliczSi�yGrawitacji()
	{
		Vector2d sumaSi� = { 0,0 };
		for (auto planeta : *m_tablicaObiekt�w)
		{
			if (planeta != this)
			{
				double wypadkowa = G_Sta�aGrawitacji * (m_masa * planeta->m_masa) / pow(odleg�o��(planeta), 2);
				double alfa = atan2(planeta->m_pos.x - m_pos.x, planeta->m_pos.y - m_pos.y);
				sumaSi�.x += wypadkowa * sin(alfa);
				sumaSi�.y += wypadkowa * cos(alfa);
			}
		}
		return sumaSi�;
	}

	virtual void obliczPozycj�(const double &czas)		//Musi by� referencja, inaczej crash???
	{
		m_F = obliczSi�yGrawitacji();


		Vector2d przysp = m_F / m_masa;			//Ca�kowanie przybli�one trapezami, dok�adne przy sta�ym przyspieszeniu

		m_pos.x += 0.5f * m_vel.x * czas;
		m_pos.y += 0.5f * m_vel.y * czas;

		m_vel += przysp * czas;

		m_pos.x += 0.5f * m_vel.x * czas;
		m_pos.y += 0.5f * m_vel.y * czas;

		setPosition(m_pos.x*G_PIKSELI_NA_METR, m_pos.y*G_PIKSELI_NA_METR);
	}

	virtual void od�wie��lad()
	{
		mu_t�lad�w.lock();
		m_�lad.push_back(Vertex(static_cast<Vector2f>(m_pos*G_PIKSELI_NA_METR), m_color));

		if (m_�lad.size() > G_D�UGO��_�LADU)
			m_�lad.erase(m_�lad.begin());

		mu_t�lad�w.unlock();
	}
};