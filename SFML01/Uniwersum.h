#pragma once

#include "stdafx.h"
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta³e.h"
#include "ZmienneGlob.h"
#include "Planeta.h"
#include "PlanetaSta³a.h"
#include "f_Ró¿ne.h"


class Uniwersum
{
protected:
	std::vector	<std::vector<Vertex>*>	m_tablicaŒladów;
	std::vector<Planeta*>				m_tablicaObiektów;
	RenderWindow						*window;
	double								m_prêdkoœæSymulacji = 1.0;
	int									m_wybranaWielkosæ = 1;


public:
	Uniwersum(RenderWindow *window) : window{window}, m_prêdkoœæSymulacji{1.0}
	{}

	~Uniwersum()
	{}

	void tFizyka()
	{
		long long numerCyklu{ 0 };
		Clock clock;
		Clock clock2;

		while (window->isOpen())
		{
			numerCyklu++;

			double czas = clock.restart().asSeconds() * m_prêdkoœæSymulacji;

			mu_tObiektów.lock();
			for (auto obiekt : m_tablicaObiektów)
			{
				obiekt->obliczPozycjê(czas);
			}
			mu_tObiektów.unlock();

			if ((numerCyklu % 1000000) == 0 && DEBUG)
				std::cout << "1e6 cykli fizyki przy " << m_tablicaObiektów.size() << " planetach zajê³o œrednio: " << clock2.restart().asMicroseconds() / 1000000.0 << " us.\n";

			//std::this_thread::sleep_for(std::chrono::microseconds(5));		//Dajmy czas na odblokowanie mutexa, dla stworzenia nowych obiektów itd.
			//jednak nie dawajmy, ten sleep dzia³a na zbyt d³ugi czas oko³o 1ms

			//Benchmark w konf. DEBUG
			//Kopiowanie tablicy:	3 obj - 7.4us, 10 obj - 28.3us		Zaleta: mutex odblokowany po skopiowaniu tablicy (po ok 5us), te¿ niestabilne jak kop. co 10
			//Bez kopiowania:		3 obj - 2.4us, 10 obj - 23us		Zaleta: dla 3 obj jest 3 razy szybciej, stabilne
			//Kop. tab. co 10 cykli: jak bez kopiowania					Wada: Niestabilne, nie wiadomo czy obiekt nie zosta³ usuniêty.
		}
	}

	void tRysowanie()
	{
		window->clear(Color::Black);

		for (Planeta *obiekt : m_tablicaObiektów)
		{
			window->draw(*obiekt);
		}

		for (std::vector<Vertex> *œlad : tablicaŒladów)
		{
			window->draw(&(œlad->at(0)), œlad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia³a
		}

		window->display();
	}

	void tRysowanie2()
	{	
		window->setActive(true);
		while (window->isOpen())
		{
			mu_tRys.lock();
			window->clear(Color::Black);

			for (Planeta *obiekt : m_tablicaObiektów)
			{
				window->draw(*obiekt);
			}

			for (std::vector<Vertex> *œlad : tablicaŒladów)
			{
				window->draw(&(œlad->at(0)), œlad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia³a
			}

			window->display();
			mu_tRys.unlock();
		}
		window->setActive(false);
	}

	void tŒlady()
	{
		Clock clock;
		long long numerCyklu{ 0 };

		while (window->isOpen())
		{
			clock.restart();
			numerCyklu++;
			//mu_tObiektów.lock();  NajwyraŸniej nie potrzebne, po usuniêciu tego mutexa ten w¹tek nie jest bardzo blokowany przez w¹tek fizyki.
			for (auto obiekt : m_tablicaObiektów)
			{
				obiekt->odœwie¿Œlad();
			}
			//mu_tObiektów.unlock();
			if ((numerCyklu % 10000) == 0 && DEBUG)
				std::cout << numerCyklu << " cykl œladów zaja³: " << clock.restart().asMicroseconds() << " us.\n";

			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	}

	Planeta* dodajPlanetê(
		const double r,
		const Vector2d pos,
		const Vector2d vel,
		const double masa,
		const Color color)
	{
		auto ptr = new Planeta(&m_tablicaObiektów, r,
			static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
			masa, color);

		mu_tObiektów.lock();
		m_tablicaObiektów.push_back(ptr);
		mu_tObiektów.unlock();

		return ptr;
	}

	PlanetaSta³a* dodajPlanetêSta³¹(
		const double r,
		const Vector2d pos,
		const Vector2d vel,
		const double masa,
		const Color color)
	{
		auto ptr = new PlanetaSta³a(&m_tablicaObiektów, r,
			static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
			masa, color);

		mu_tObiektów.lock();
		m_tablicaObiektów.push_back(ptr);
		mu_tObiektów.unlock();

		return ptr;
	}

	void usunObiektKursor()
	{
		Vector2f kursor = window->mapPixelToCoords(Mouse::getPosition(*window));

		Planeta *wskazany = nullptr;

		for (Planeta *obiekt : m_tablicaObiektów)
		{
			Vector2f œrodekPlanety = obiekt->getPosition();
			float odle = odl2(kursor, œrodekPlanety);

			if (odle <= obiekt->getRadius())
			{
				wskazany = obiekt;
				break;
			}
		}

		delete wskazany;

	}

	void przesuñWidokOkna(Vector2f a)
	{
		View view = window->getView();
		view.move(a * view.getSize().x / 160.0f);
		window->setView(view);
	}

	void wybierzRozmiar(int i)
	{
		m_wybranaWielkosæ += i;
		std::cout << "Wybrano masê planety " << pow(10.0, m_wybranaWielkosæ) << " kg.\n";
	}

	void wybierzPrêdkoœæSymulacji(double i)
	{
		m_prêdkoœæSymulacji *= i;
		std::cout << "Wybrano prêdkoœæ symulacji " << m_prêdkoœæSymulacji << "x.\n";
	}

	void narysujObiekt()
	{
		//std::cout << "Mouse position: " << Mouse::getPosition(*window).x << " " << Mouse::getPosition(*window).y <<"\n";
		Vector2f pocz¹tek = window->mapPixelToCoords(Mouse::getPosition(*window));
		std::cout << pocz¹tek.x << " " << pocz¹tek.y << "\n";
		while (Mouse::isButtonPressed(Mouse::Left))
		{
		}
		Vector2f koniec = window->mapPixelToCoords(Mouse::getPosition(*window));
		Vector2f prêdkoœæ = { (koniec.x - pocz¹tek.x)*1.0f, (koniec.y - pocz¹tek.y)*1.0f };


		double masa = pow(10.0, m_wybranaWielkosæ);
		double r = cbrt(masa);

		Planeta* ptr = dodajPlanetê(r, static_cast<Vector2d>(pocz¹tek) / G_PIKSELI_NA_METR, static_cast<Vector2d>(prêdkoœæ) / G_PIKSELI_NA_METR, masa, Color(rand(0, 255), rand(0, 255), rand(0, 255)));

		if (DEBUG)
			std::cout << "Narysowano obiekt w adresie: " << ptr << ",pozycja " << ptr->getPosition().x << " " << ptr->getPosition().y <<" o predkosci " << prêdkoœæ.x << " " << prêdkoœæ.y << "\n";
	}

	void zoomScroll(Event &event)
	{
		View view = window->getView();

		Vector2f kursorCoord = window->mapPixelToCoords(Mouse::getPosition(*window));
		Vector2f œrodek = view.getCenter();

		float zmianaSkali = -G_ZOOM_SENS * event.mouseWheelScroll.x;

		if (zmianaSkali > G_ZOOM_MAXSPEED)
			zmianaSkali = G_ZOOM_MAXSPEED;

		if (event.mouseWheelScroll.delta < 0)
			zmianaSkali = -zmianaSkali;

		œrodek -= zmianaSkali * (kursorCoord - œrodek);

		view.zoom(1.0f + zmianaSkali);
		view.setCenter(œrodek);
		window->setView(view);

	}

	void debugujPlanetê()
	{
		Planeta* planeta = m_tablicaObiektów[0];
		std::cout <<"###################\n" << 
			"Radius: " << planeta->getRadius() << "\n"
			<< "Planeta pos: " << planeta->getPosition().x << " " << planeta->getPosition().y << "\n"
			<< "PlanetatoPixel: " << window->mapCoordsToPixel(planeta->getPosition()).x << " " << window->mapCoordsToPixel(planeta->getPosition()).y << "\n"
			<< "Kursor: " << Mouse::getPosition(*window).x << " " << Mouse::getPosition(*window).y << "\n"
			<< "Kursortocoord: " << window->mapPixelToCoords(Mouse::getPosition(*window)).x << " " << window->mapPixelToCoords(Mouse::getPosition(*window)).y << "\n"
			<< "Odleg³oœæ :" << odl2(planeta->getPosition(), window->mapPixelToCoords(Mouse::getPosition(*window))) << "\n";
	}

	void tHandleEvents()
	{
		Event event;
		while (window->pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:		//Zamknij okno
				window->close(); 
				break;
			case Event::Resized:	//Zmieñ rozmiar okna
				G_WINWIDTH = event.size.width;
				G_WINHEIGHT = event.size.height;
				std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::Up:
					przesuñWidokOkna(Vector2f(0, -10));
					break;
				case Keyboard::Down:
					przesuñWidokOkna(Vector2f(0, 10));
					break;
				case Keyboard::Left:
					przesuñWidokOkna(Vector2f(-10, 0));
					break;
				case Keyboard::Right:
					przesuñWidokOkna(Vector2f(10, 0));
					break;
				case Keyboard::Subtract:
					wybierzRozmiar(-1);
					break;
				case Keyboard::Add:
					wybierzRozmiar(1);
					break;
				case Keyboard::Comma:
					wybierzPrêdkoœæSymulacji(0.5);
					break;
				case Keyboard::Period:
					wybierzPrêdkoœæSymulacji(2.0);
					break;
				case Keyboard::D:
					debugujPlanetê();
					break;
				default:
					break;
				}
				break;
			case Event::MouseButtonPressed:
				switch (event.mouseButton.button)
				{
				case Mouse::Left:
					narysujObiekt();
					break;
				case Mouse::Right:
					usunObiektKursor();
					break;
				}
				break;
			case Event::MouseWheelScrolled:
				zoomScroll( event);
				break;
			default:
				break;
			}
		}
	}

};