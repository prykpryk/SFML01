#pragma once

//#include "stdafx.h"
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"
#include "ZmienneGlob.h"

class Planeta : public sf::CircleShape
{
protected:

	Vector2d m_F;
	double m_R;
	sf::Color m_color;


public:
	std::vector<sf::Vertex> m_�lad;
	double m_masa;
	Vector2d m_pos;
	bool m_zablokowana;
	Vector2d m_vel;


	Planeta(const double masa, const double r, const Vector2d pos, const Vector2d vel, const sf::Color color, bool zablokowana);

	~Planeta();

	double odleg�o��(const Planeta * planeta) const;

	static double odleg�o��(const Planeta * a, const Planeta * b);

	//Vector2d obliczSi�yGrawitacji();

	//virtual void obliczPozycj�(const double & czas);

	virtual void od�wie��lad();

	void od�wie�Kszta�t(sf::RenderWindow * window);

	bool sprawd�Kolizj�(const Planeta * planeta);

	static bool sprawd�Kolizj�(const Planeta * a, const Planeta * b);

	Planeta * znajd�Kolizje(std::vector<Planeta>& tablicaObjekt�w);

};