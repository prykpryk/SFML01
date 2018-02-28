//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"

#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_R�ne.h"

Planeta* Uniwersum::planetaKursor()
{	//Funkcja wska�e obiekt pod kursorem, je�eli jest wi�cej ni� 1 to b�dzie to obiekt najni�ej.
	sf::Vector2f kursor = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	for (Planeta & obiekt : m_tablicaObiekt�w)
	{
		sf::Vector2f �rodekPlanety = obiekt.getPosition();
		float odle = odl2(kursor, �rodekPlanety);

		if (odle <= obiekt.getRadius())
		{
			//std::cout << "Znaleziono\n";
			return &obiekt;
		}
	}
	//std::cout << "Nie znaleziono nic\n";
	return nullptr;
}

void Uniwersum::usunObiektKursor()
{
	usu�Planet�(*planetaKursor());
}

void Uniwersum::przesu�WidokOkna(sf::Vector2f a)
{
	sf::View view = window->getView();
	view.move(a * view.getSize().x / 160.0f);
	window->setView(view);
}

void Uniwersum::wybierzRozmiar(int i)
{
	m_wybranaWielkos� += i;
	std::cout << "Wybrano mas� planety " << pow(10.0, m_wybranaWielkos�) << " kg.\n";
}

void Uniwersum::wybierzPr�dko��Symulacji(double i)
{
	m_pr�dko��Symulacji *= i;
	std::cout << "Wybrano pr�dko�� symulacji " << m_pr�dko��Symulacji << "x.\n";
}

void Uniwersum::narysujObiekt()
{

	sf::Vector2f pocz�tek = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
	}

	sf::Vector2f koniec = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f pr�dko�� = { (koniec.x - pocz�tek.x)*1.0f, (koniec.y - pocz�tek.y)*1.0f };

	double masa = pow(10.0, m_wybranaWielkos�);
	double r = cbrt(masa);

	Planeta* ptr = dodajPlanet�(masa, r, static_cast<Vector2d>(pocz�tek), static_cast<Vector2d>(pr�dko��), sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << ",pozycja " << ptr->getPosition().x << " " << ptr->getPosition().y << " o predkosci " << pr�dko��.x << " " << pr�dko��.y << "\n";
}

void Uniwersum::zoomScroll(sf::Event &event)
{
	sf::View view = window->getView();

	sf::Vector2f kursorCoord = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f �rodek = view.getCenter();

	float zmianaSkali = -G_ZOOM_SENS * event.mouseWheelScroll.x;

	if (zmianaSkali > G_ZOOM_MAXSPEED)
		zmianaSkali = G_ZOOM_MAXSPEED;

	if (event.mouseWheelScroll.delta < 0)
		zmianaSkali = -zmianaSkali;

	�rodek -= zmianaSkali * (kursorCoord - �rodek);

	view.zoom(1.0f + zmianaSkali);
	if (m_�ledzonaPlaneta == nullptr)
		view.setCenter(�rodek);
	window->setView(view);

	od�wie�Kszta�y();

}

void Uniwersum::debugujPlanet�()
{
	if (!DEBUG) return;
	Planeta planeta = m_tablicaObiekt�w[0];
	std::cout << "###################\n" <<
		"Radius: " << planeta.getRadius() << "\n"
		<< "Planeta pos: " << planeta.getPosition().x << " " << planeta.getPosition().y << "\n"
		<< "PlanetatoPixel: " << window->mapCoordsToPixel(planeta.getPosition()).x << " " << window->mapCoordsToPixel(planeta.getPosition()).y << "\n"
		<< "Kursor: " << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << "\n"
		<< "Kursortocoord: " << window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x << " " << window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y << "\n"
		<< "Odleg�o�� :" << odl2(planeta.getPosition(), window->mapPixelToCoords(sf::Mouse::getPosition(*window))) << "\n"
		<< window->getView().getSize().x << "\n";
}

void Uniwersum::handleEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:		//Zamknij okno
			window->close();
			break;
		case sf::Event::Resized:	//Zmie� rozmiar okna
			zmie�RozmiarWidoku(event.size.width, event.size.height);
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				przesu�WidokOkna(sf::Vector2f(0, -10));
				break;
			case sf::Keyboard::Down:
				przesu�WidokOkna(sf::Vector2f(0, 10));
				break;
			case sf::Keyboard::Left:
				przesu�WidokOkna(sf::Vector2f(-10, 0));
				break;
			case sf::Keyboard::Right:
				przesu�WidokOkna(sf::Vector2f(10, 0));
				break;
			case sf::Keyboard::Subtract:
				wybierzRozmiar(-1);
				break;
			case sf::Keyboard::Add:
				wybierzRozmiar(1);
				break;
			case sf::Keyboard::Comma:
				wybierzPr�dko��Symulacji(0.5);
				break;
			case sf::Keyboard::Period:
				wybierzPr�dko��Symulacji(2.0);
				break;
			case sf::Keyboard::D:
				debugujPlanet�();
				break;
			case sf::Keyboard::P:
				prze��czPauz�();
				break;
			case sf::Keyboard::K:
				kolizje();
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				narysujObiekt();
				break;
			case sf::Mouse::Right:
				usunObiektKursor();
				break;
			case sf::Mouse::Middle:
				ustaw�ledzon�Planet�Kursor();
				break;
			}
			break;
		case sf::Event::MouseWheelScrolled:
			zoomScroll(event);
			break;
		default:
			break;
		}
	}
}

void Uniwersum::od�wie�Kszta�y()
{
	for (Planeta & obiekt : m_tablicaObiekt�w)
	{
		obiekt.od�wie�Kszta�t(window);
	}
}

void Uniwersum::zmie�RozmiarWidoku(int w, int h)
{
	sf::View view = window->getView();

	sf::Vector2f rozmiarStary = view.getSize();

	view.setSize(rozmiarStary.x, rozmiarStary.x * h / w);

	window->setView(view);

	od�wie�Kszta�y();
}

void Uniwersum::�led�Planet�()
{
	if (m_�ledzonaPlaneta == nullptr) return;

	sf::View view = window->getView();
	view.setCenter(static_cast<float>(m_�ledzonaPlaneta->m_pos.x), static_cast<float>(m_�ledzonaPlaneta->m_pos.y));
	window->setView(view);
}

void Uniwersum::ustaw�ledzon�Planet�(Planeta* planeta)
{
	m_�ledzonaPlaneta = planeta;
}

void Uniwersum::ustaw�ledzon�Planet�Kursor()
{
	ustaw�ledzon�Planet�(planetaKursor());
}

bool Uniwersum::prze��czPauz�()
{
	m_pr�dko��Symulacji = -m_pr�dko��Symulacji;
	if (m_pr�dko��Symulacji < 0)
		return true;
	else
		return false;
}