#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"
#include "ZmienneGlob.h"

#include "Planeta.h"

Planeta::Planeta(
	std::vector<Planeta*> *tablicaObiekt�w,
	const double masa,
	const double r,
	const Vector2d pos = { 0, 0 },
	const Vector2d vel = { 0,0 },
	const sf::Color color = sf::Color::White,
	bool zablokowana = false)
	: CircleShape{ static_cast<float>(r),16 }, m_pos{ pos }, m_vel{ vel }, m_tablicaObiekt�w{ tablicaObiekt�w },
	m_R{ r }, m_masa{ masa }, m_color{ color }, m_zablokowana{ zablokowana }
{
	setPosition(static_cast<sf::Vector2f>(m_pos));
	setOrigin(static_cast<float>(r), static_cast<float>(r));
	setFillColor(m_color);
	//m_�lad.setPrimitiveType(LinesStrip);

	od�wie��lad();
	tablica�lad�w.push_back(&m_�lad);
};

Planeta::~Planeta()
{
}

double Planeta::odleg�o��(const Planeta *planeta)
{
	return sqrt(pow(m_pos.x - planeta->m_pos.x, 2) + pow(m_pos.y - planeta->m_pos.y, 2));
}

double Planeta::odleg�o��(const Planeta *a, const Planeta *b)
{
	return sqrt(pow(b->m_pos.y - a->m_pos.x, 2) + pow(b->m_pos.y - a->m_pos.y, 2));
}

Vector2d Planeta::obliczSi�yGrawitacji()
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

void Planeta::obliczPozycj�(const double &czas)		//Musi by� referencja, inaczej crash???
{
	if (m_zablokowana) return;

	m_F = obliczSi�yGrawitacji();


	Vector2d przysp = m_F / m_masa;			//Ca�kowanie przybli�one trapezami, dok�adne przy sta�ym przyspieszeniu

	m_pos.x += 0.5f * m_vel.x * czas;
	m_pos.y += 0.5f * m_vel.y * czas;

	m_vel += przysp * czas;

	m_pos.x += 0.5f * m_vel.x * czas;
	m_pos.y += 0.5f * m_vel.y * czas;

	setPosition(static_cast<float>(m_pos.x), static_cast<float>(m_pos.y));
}

void Planeta::od�wie��lad()
{
	mu_t�lad�w.lock();
	m_�lad.push_back(sf::Vertex(static_cast<sf::Vector2f>(m_pos), m_color));

	if (m_�lad.size() > G_D�UGO��_�LADU)
		m_�lad.erase(m_�lad.begin());

	mu_t�lad�w.unlock();
}

void Planeta::od�wie�Kszta�t(sf::RenderWindow *window)
{
	float wielko�� = window->getView().getSize().x / window->getSize().x;		//B�dzie dziwnie przy oknie ma�o kwadratowym.
	int x = static_cast<int>(getRadius() / wielko��) * 2;

	if (x < 6)
		x = 6;
	if (x > 100)
		x = 100;

	setPointCount(x);
}

bool Planeta::sprawd�Kolizj�(const Planeta *planeta)
{
	if (odleg�o��(planeta) < (m_R + planeta->m_R))
		return true;

	return false;
}

bool Planeta::sprawd�Kolizj�(const Planeta *a, const Planeta *b)
{
	if (odleg�o��(a, b) < (b->m_R + a->m_R))
		return true;

	return false;
}

Planeta*  Planeta::znajd�Kolizje(const std::vector<Planeta*> &tablicaObjekt�w)
{
	//Zwraca planet� koliduj�c� z t� planet�.

	for (auto a = tablicaObjekt�w.begin(); a != tablicaObjekt�w.end(); a++)
	{
		if (*a == this)
			continue;
		if (sprawd�Kolizj�(*a))
			return *a;
	}

	return nullptr;
}