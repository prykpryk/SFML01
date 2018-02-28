#pragma once

//#include "stdafx.h"
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"
#include "ZmienneGlob.h"

class Planeta : public sf::CircleShape
{
protected:

	Vector2d m_vel;
	Vector2d m_F;
	double m_R;
	std::vector<Planeta*> *m_tablicaObiektów;
	sf::Color m_color;
	bool m_zablokowana;

public:
	std::vector<sf::Vertex> m_œlad;
	double m_masa;
	Vector2d m_pos;



	Planeta(std::vector<Planeta*>* tablicaObiektów, const double masa, const double r, const Vector2d pos, const Vector2d vel, const sf::Color color, bool zablokowana);

	~Planeta();

	double odleg³oœæ(const Planeta * planeta);

	static double odleg³oœæ(const Planeta * a, const Planeta * b);

	Vector2d obliczSi³yGrawitacji();

	virtual void obliczPozycjê(const double & czas);

	virtual void odœwie¿Œlad();

	void odœwie¿Kszta³t(sf::RenderWindow * window);

	bool sprawdŸKolizjê(const Planeta * planeta);

	static bool sprawdŸKolizjê(const Planeta * a, const Planeta * b);

};