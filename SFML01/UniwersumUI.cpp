//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta³e.h"

#include "Planeta.h"

#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_Ró¿ne.h"

Planeta* Uniwersum::planetaKursor()
{	//Funkcja wska¿e obiekt pod kursorem, je¿eli jest wiêcej ni¿ 1 to bêdzie to obiekt najni¿ej.
	sf::Vector2f kursor = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	for (Planeta & obiekt : m_tablicaObiektów)
	{
		sf::Vector2f œrodekPlanety = obiekt.getPosition();
		float odle = odl2(kursor, œrodekPlanety);

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
	usuñPlanetê(*planetaKursor());
}

void Uniwersum::przesuñWidokOkna(sf::Vector2f a)
{
	sf::View view = window->getView();
	view.move(a * view.getSize().x / 160.0f);
	window->setView(view);
}

void Uniwersum::wybierzRozmiar(int i)
{
	m_wybranaWielkosæ += i;
	std::cout << "Wybrano masê planety " << pow(10.0, m_wybranaWielkosæ) << " kg.\n";
}

void Uniwersum::wybierzPrêdkoœæSymulacji(double i)
{
	m_prêdkoœæSymulacji *= i;
	std::cout << "Wybrano prêdkoœæ symulacji " << m_prêdkoœæSymulacji << "x.\n";
}

void Uniwersum::narysujObiekt()
{

	sf::Vector2f pocz¹tek = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
	}

	sf::Vector2f koniec = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f prêdkoœæ = { (koniec.x - pocz¹tek.x)*1.0f, (koniec.y - pocz¹tek.y)*1.0f };

	double masa = pow(10.0, m_wybranaWielkosæ);
	double r = cbrt(masa);

	Planeta* ptr = dodajPlanetê(masa, r, static_cast<Vector2d>(pocz¹tek), static_cast<Vector2d>(prêdkoœæ), sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));

	if (DEBUG)
		std::cout << "Narysowano obiekt w adresie: " << ptr << ",pozycja " << ptr->getPosition().x << " " << ptr->getPosition().y << " o predkosci " << prêdkoœæ.x << " " << prêdkoœæ.y << "\n";
}

void Uniwersum::zoomScroll(sf::Event &event)
{
	sf::View view = window->getView();

	sf::Vector2f kursorCoord = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f œrodek = view.getCenter();

	float zmianaSkali = -G_ZOOM_SENS * event.mouseWheelScroll.x;

	if (zmianaSkali > G_ZOOM_MAXSPEED)
		zmianaSkali = G_ZOOM_MAXSPEED;

	if (event.mouseWheelScroll.delta < 0)
		zmianaSkali = -zmianaSkali;

	œrodek -= zmianaSkali * (kursorCoord - œrodek);

	view.zoom(1.0f + zmianaSkali);
	if (m_œledzonaPlaneta == nullptr)
		view.setCenter(œrodek);
	window->setView(view);

	odœwie¿Kszta³y();

}

void Uniwersum::debugujPlanetê()
{
	if (!DEBUG) return;
	Planeta planeta = m_tablicaObiektów[0];
	std::cout << "###################\n" <<
		"Radius: " << planeta.getRadius() << "\n"
		<< "Planeta pos: " << planeta.getPosition().x << " " << planeta.getPosition().y << "\n"
		<< "PlanetatoPixel: " << window->mapCoordsToPixel(planeta.getPosition()).x << " " << window->mapCoordsToPixel(planeta.getPosition()).y << "\n"
		<< "Kursor: " << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << "\n"
		<< "Kursortocoord: " << window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x << " " << window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y << "\n"
		<< "Odleg³oœæ :" << odl2(planeta.getPosition(), window->mapPixelToCoords(sf::Mouse::getPosition(*window))) << "\n"
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
		case sf::Event::Resized:	//Zmieñ rozmiar okna
			zmieñRozmiarWidoku(event.size.width, event.size.height);
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				przesuñWidokOkna(sf::Vector2f(0, -10));
				break;
			case sf::Keyboard::Down:
				przesuñWidokOkna(sf::Vector2f(0, 10));
				break;
			case sf::Keyboard::Left:
				przesuñWidokOkna(sf::Vector2f(-10, 0));
				break;
			case sf::Keyboard::Right:
				przesuñWidokOkna(sf::Vector2f(10, 0));
				break;
			case sf::Keyboard::Subtract:
				wybierzRozmiar(-1);
				break;
			case sf::Keyboard::Add:
				wybierzRozmiar(1);
				break;
			case sf::Keyboard::Comma:
				wybierzPrêdkoœæSymulacji(0.5);
				break;
			case sf::Keyboard::Period:
				wybierzPrêdkoœæSymulacji(2.0);
				break;
			case sf::Keyboard::D:
				debugujPlanetê();
				break;
			case sf::Keyboard::P:
				prze³¹czPauzê();
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
				ustawŒledzon¹PlanetêKursor();
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

void Uniwersum::odœwie¿Kszta³y()
{
	for (Planeta & obiekt : m_tablicaObiektów)
	{
		obiekt.odœwie¿Kszta³t(window);
	}
}

void Uniwersum::zmieñRozmiarWidoku(int w, int h)
{
	sf::View view = window->getView();

	sf::Vector2f rozmiarStary = view.getSize();

	view.setSize(rozmiarStary.x, rozmiarStary.x * h / w);

	window->setView(view);

	odœwie¿Kszta³y();
}

void Uniwersum::œledŸPlanetê()
{
	if (m_œledzonaPlaneta == nullptr) return;

	sf::View view = window->getView();
	view.setCenter(static_cast<float>(m_œledzonaPlaneta->m_pos.x), static_cast<float>(m_œledzonaPlaneta->m_pos.y));
	window->setView(view);
}

void Uniwersum::ustawŒledzon¹Planetê(Planeta* planeta)
{
	m_œledzonaPlaneta = planeta;
}

void Uniwersum::ustawŒledzon¹PlanetêKursor()
{
	ustawŒledzon¹Planetê(planetaKursor());
}

bool Uniwersum::prze³¹czPauzê()
{
	m_prêdkoœæSymulacji = -m_prêdkoœæSymulacji;
	if (m_prêdkoœæSymulacji < 0)
		return true;
	else
		return false;
}