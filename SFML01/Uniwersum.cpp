//#include "stdafx.h"
#include "Biblioteki.h"

#include "Definicje.h"

#include "Sta�e.h"

#include "Planeta.h"
#include "PlanetaSta�a.h"
#include "Uniwersum.h"
#include "ZmienneGlob.h"
#include "f_R�ne.h"


void Uniwersum::tFizyka()
{
	long long numerCyklu{ 0 };
	sf::Clock clock;
	sf::Clock clock2;

	while (window->isOpen())
	{
		numerCyklu++;

		double czas = clock.restart().asSeconds() * m_pr�dko��Symulacji;

		mu_tObiekt�w.lock();
		for (auto obiekt : m_tablicaObiekt�w)
		{
			obiekt->obliczPozycj�(czas);
		}
		mu_tObiekt�w.unlock();

		if ((numerCyklu % 10000000) == 0 && DEBUG)
			std::cout << "1e7 cykli fizyki przy " << m_tablicaObiekt�w.size() << " planetach zaj�o �rednio: " << clock2.restart().asMicroseconds() / 10000000.0 << " us.\n";

		//std::this_thread::sleep_for(std::chrono::microseconds(5));		//Dajmy czas na odblokowanie mutexa, dla stworzenia nowych obiekt�w itd.
		//jednak nie dawajmy, ten sleep dzia�a na zbyt d�ugi czas oko�o 1ms

		//Benchmark w konf. DEBUG
		//Kopiowanie tablicy:	3 obj - 7.4us, 10 obj - 28.3us		Zaleta: mutex odblokowany po skopiowaniu tablicy (po ok 5us), te� niestabilne jak kop. co 10
		//Bez kopiowania:		3 obj - 2.4us, 10 obj - 23us		Zaleta: dla 3 obj jest 3 razy szybciej, stabilne
		//Kop. tab. co 10 cykli: jak bez kopiowania					Wada: Niestabilne, nie wiadomo czy obiekt nie zosta� usuni�ty.
	}
}

void Uniwersum::tRysowanie()
{
	window->clear(sf::Color::Black);

	for (Planeta *obiekt : m_tablicaObiekt�w)
	{
		window->draw(*obiekt);
	}

	for (std::vector<sf::Vertex> *�lad : tablica�lad�w)
	{
		window->draw(&(�lad->at(0)), �lad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia�a
	}

	window->display();
}

void Uniwersum::tRysowanie2()
{
	window->setActive(true);
	while (window->isOpen())
	{
		mu_tRys.lock();
		window->clear(sf::Color::Black);

		for (Planeta *obiekt : m_tablicaObiekt�w)
		{
			window->draw(*obiekt);
		}

		for (std::vector<sf::Vertex> *�lad : tablica�lad�w)
		{
			window->draw(&(�lad->at(0)), �lad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia�a
		}

		window->display();
		mu_tRys.unlock();
	}
	window->setActive(false);
}

void Uniwersum::t�lady()
{
	sf::Clock clock;
	long long numerCyklu{ 0 };

	while (window->isOpen())
	{
		clock.restart();
		numerCyklu++;
		//mu_tObiekt�w.lock();  Najwyra�niej nie potrzebne, po usuni�ciu tego mutexa ten w�tek nie jest bardzo blokowany przez w�tek fizyki.
		for (auto obiekt : m_tablicaObiekt�w)
		{
			obiekt->od�wie��lad();
		}
		//mu_tObiekt�w.unlock();
		if ((numerCyklu % 100) == 0 && DEBUG)
			std::cout << numerCyklu << " cykl �lad�w zaja�: " << clock.restart().asMicroseconds() << " us.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}

Planeta* Uniwersum::dodajPlanet�(
	const double r,
	const Vector2d pos,
	const Vector2d vel,
	const double masa,
	const sf::Color color)
{
	auto ptr = new Planeta(&m_tablicaObiekt�w, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		masa, color);

	mu_tObiekt�w.lock();
	m_tablicaObiekt�w.push_back(ptr);
	mu_tObiekt�w.unlock();

	return ptr;
}

PlanetaSta�a* Uniwersum::dodajPlanet�Sta��(
	const double r,
	const Vector2d pos,
	const Vector2d vel,
	const double masa,
	const sf::Color color)
{
	auto ptr = new PlanetaSta�a(&m_tablicaObiekt�w, r,
		static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
		masa, color);

	mu_tObiekt�w.lock();
	m_tablicaObiekt�w.push_back(ptr);
	mu_tObiekt�w.unlock();

	return ptr;
}

void Uniwersum::usunObiektKursor()
{
	sf::Vector2f kursor = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	Planeta *wskazany = nullptr;

	for (Planeta *obiekt : m_tablicaObiekt�w)
	{
		sf::Vector2f �rodekPlanety = obiekt->getPosition();
		float odle = odl2(kursor, �rodekPlanety);

		if (odle <= obiekt->getRadius())
		{
			wskazany = obiekt;
			break;
		}
	}

	delete wskazany;

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
	//std::cout << "Mouse position: " << Mouse::getPosition(*window).x << " " << Mouse::getPosition(*window).y <<"\n";
	sf::Vector2f pocz�tek = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	std::cout << pocz�tek.x << " " << pocz�tek.y << "\n";
	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
	}
	sf::Vector2f koniec = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f pr�dko�� = { (koniec.x - pocz�tek.x)*1.0f, (koniec.y - pocz�tek.y)*1.0f };


	double masa = pow(10.0, m_wybranaWielkos�);
	double r = cbrt(masa);

	Planeta* ptr = dodajPlanet�(r, static_cast<Vector2d>(pocz�tek) / G_PIKSELI_NA_METR, static_cast<Vector2d>(pr�dko��) / G_PIKSELI_NA_METR, masa, sf::Color(rand(0, 255), rand(0, 255), rand(0, 255)));

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
	view.setCenter(�rodek);
	window->setView(view);

}

void Uniwersum::debugujPlanet�()
{
	Planeta* planeta = m_tablicaObiekt�w[0];
	std::cout << "###################\n" <<
		"Radius: " << planeta->getRadius() << "\n"
		<< "Planeta pos: " << planeta->getPosition().x << " " << planeta->getPosition().y << "\n"
		<< "PlanetatoPixel: " << window->mapCoordsToPixel(planeta->getPosition()).x << " " << window->mapCoordsToPixel(planeta->getPosition()).y << "\n"
		<< "Kursor: " << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << "\n"
		<< "Kursortocoord: " << window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x << " " << window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y << "\n"
		<< "Odleg�o�� :" << odl2(planeta->getPosition(), window->mapPixelToCoords(sf::Mouse::getPosition(*window))) << "\n";
}

void Uniwersum::tHandleEvents()
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
			G_WINWIDTH = event.size.width;
			G_WINHEIGHT = event.size.height;
			std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
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
