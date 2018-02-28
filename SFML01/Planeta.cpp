#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"
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
	//m_œlad.setPrimitiveType(LinesStrip);

	odœwie¿Œlad();
	tablicaŒladów.push_back(&m_œlad);
};

Planeta::~Planeta()
{
}

double Planeta::odleg³oœæ(const Planeta *planeta) const
{
	return sqrt(pow(m_pos.x - planeta->m_pos.x, 2) + pow(m_pos.y - planeta->m_pos.y, 2));
}

double Planeta::odleg³oœæ(const Planeta *a, const Planeta *b)
{
	return sqrt(pow(b->m_pos.y - a->m_pos.x, 2) + pow(b->m_pos.y - a->m_pos.y, 2));
}

void Planeta::odœwie¿Œlad()
{
	mu_tŒladów.lock();
	m_œlad.push_back(sf::Vertex(static_cast<sf::Vector2f>(m_pos), m_color));

	if (m_œlad.size() > G_D£UGOŒÆ_ŒLADU)
		m_œlad.erase(m_œlad.begin());

	mu_tŒladów.unlock();
}

void Planeta::odœwie¿Kszta³t(sf::RenderWindow *window)
{
	float wielkoœæ = window->getView().getSize().x / window->getSize().x;		//Bêdzie dziwnie przy oknie ma³o kwadratowym.
	int x = static_cast<int>(getRadius() / wielkoœæ) * 2;

	if (x < 6)
		x = 6;
	if (x > 100)
		x = 100;

	setPointCount(x);
}

bool Planeta::sprawdŸKolizjê(const Planeta *planeta)
{
	if (odleg³oœæ(planeta) < (m_R + planeta->m_R))
		return true;

	return false;
}

bool Planeta::sprawdŸKolizjê(const Planeta *a, const Planeta *b)
{
	if (odleg³oœæ(a, b) < (b->m_R + a->m_R))
		return true;

	return false;
}

Planeta*  Planeta::znajdŸKolizje(std::vector<Planeta> & tablicaObjektów)
{
	//Zwraca planetê koliduj¹c¹ z t¹ planet¹.

	for (Planeta & a : tablicaObjektów)
	{
		if (&a == this)
			continue;
		if (sprawdŸKolizjê(&a))
			return &a;
	}

	return nullptr;
}