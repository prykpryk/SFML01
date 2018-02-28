#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"
#include "ZmienneGlob.h"

#include "Planeta.h"

Planeta::Planeta(
	const double masa,
	const double r,
	const Vector2d pos = { 0, 0 },
	const Vector2d vel = { 0,0 },
	const sf::Color color = sf::Color::White,
	bool zablokowana = false)
	: CircleShape{ static_cast<float>(r),16 }, m_pos{ pos }, m_vel{ vel },
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

double Planeta::odleg�o��(const Planeta *planeta) const
{
	return sqrt(pow(m_pos.x - planeta->m_pos.x, 2) + pow(m_pos.y - planeta->m_pos.y, 2));
}

double Planeta::odleg�o��(const Planeta *a, const Planeta *b)
{
	return sqrt(pow(b->m_pos.y - a->m_pos.x, 2) + pow(b->m_pos.y - a->m_pos.y, 2));
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

Planeta*  Planeta::znajd�Kolizje(std::vector<Planeta> & tablicaObjekt�w)
{
	//Zwraca planet� koliduj�c� z t� planet�.

	for (Planeta & a : tablicaObjekt�w)
	{
		if (&a == this)
			continue;
		if (sprawd�Kolizj�(&a))
			return &a;
	}

	return nullptr;
}