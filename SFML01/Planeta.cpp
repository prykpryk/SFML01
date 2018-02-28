#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"
#include "ZmienneGlob.h"

#include "Planeta.h"

Planeta::Planeta(
	std::vector<Planeta*> *tablicaObiektów,
	const double masa,
	const double r,
	const Vector2d pos = { 0, 0 },
	const Vector2d vel = { 0,0 },
	const sf::Color color = sf::Color::White,
	bool zablokowana = false)
	: CircleShape{ static_cast<float>(r),16 }, m_pos{ pos }, m_vel{ vel }, m_tablicaObiektów{ tablicaObiektów },
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

double Planeta::odleg³oœæ(const Planeta *planeta)
{
	return sqrt(pow(m_pos.x - planeta->m_pos.x, 2) + pow(m_pos.y - planeta->m_pos.y, 2));
}

double Planeta::odleg³oœæ(const Planeta *a, const Planeta *b)
{
	return sqrt(pow(b->m_pos.y - a->m_pos.x, 2) + pow(b->m_pos.y - a->m_pos.y, 2));
}

Vector2d Planeta::obliczSi³yGrawitacji()
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

void Planeta::obliczPozycjê(const double &czas)		//Musi byæ referencja, inaczej crash???
{
	if (m_zablokowana) return;

	m_F = obliczSi³yGrawitacji();


	Vector2d przysp = m_F / m_masa;			//Ca³kowanie przybli¿one trapezami, dok³adne przy sta³ym przyspieszeniu

	m_pos.x += 0.5f * m_vel.x * czas;
	m_pos.y += 0.5f * m_vel.y * czas;

	m_vel += przysp * czas;

	m_pos.x += 0.5f * m_vel.x * czas;
	m_pos.y += 0.5f * m_vel.y * czas;

	setPosition(static_cast<float>(m_pos.x), static_cast<float>(m_pos.y));
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

Planeta*  Planeta::znajdŸKolizje(const std::vector<Planeta*> &tablicaObjektów)
{
	//Zwraca planetê koliduj¹c¹ z t¹ planet¹.

	for (auto a = tablicaObjektów.begin(); a != tablicaObjektów.end(); a++)
	{
		if (*a == this)
			continue;
		if (sprawdŸKolizjê(*a))
			return *a;
	}

	return nullptr;
}