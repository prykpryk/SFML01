#pragma once

#include "stdafx.h"
#include "Biblioteki.h"
#include "Definicje.h"
#include "Sta�e.h"
#include "ZmienneGlob.h"
#include "Planeta.h"
#include "PlanetaSta�a.h"
#include "f_R�ne.h"


class Uniwersum
{
protected:
	std::vector	<std::vector<Vertex>*>	m_tablica�lad�w;
	std::vector<Planeta*>				m_tablicaObiekt�w;
	RenderWindow						*window;
	double								m_pr�dko��Symulacji = 1.0;
	int									m_wybranaWielkos� = 1;


public:
	Uniwersum(RenderWindow *window) : window{window}, m_pr�dko��Symulacji{1.0}
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

			double czas = clock.restart().asSeconds() * m_pr�dko��Symulacji;

			mu_tObiekt�w.lock();
			for (auto obiekt : m_tablicaObiekt�w)
			{
				obiekt->obliczPozycj�(czas);
			}
			mu_tObiekt�w.unlock();

			if ((numerCyklu % 1000000) == 0 && DEBUG)
				std::cout << "1e6 cykli fizyki przy " << m_tablicaObiekt�w.size() << " planetach zaj�o �rednio: " << clock2.restart().asMicroseconds() / 1000000.0 << " us.\n";

			//std::this_thread::sleep_for(std::chrono::microseconds(5));		//Dajmy czas na odblokowanie mutexa, dla stworzenia nowych obiekt�w itd.
			//jednak nie dawajmy, ten sleep dzia�a na zbyt d�ugi czas oko�o 1ms

			//Benchmark w konf. DEBUG
			//Kopiowanie tablicy:	3 obj - 7.4us, 10 obj - 28.3us		Zaleta: mutex odblokowany po skopiowaniu tablicy (po ok 5us), te� niestabilne jak kop. co 10
			//Bez kopiowania:		3 obj - 2.4us, 10 obj - 23us		Zaleta: dla 3 obj jest 3 razy szybciej, stabilne
			//Kop. tab. co 10 cykli: jak bez kopiowania					Wada: Niestabilne, nie wiadomo czy obiekt nie zosta� usuni�ty.
		}
	}

	void tRysowanie()
	{
		window->clear(Color::Black);

		for (Planeta *obiekt : m_tablicaObiekt�w)
		{
			window->draw(*obiekt);
		}

		for (std::vector<Vertex> *�lad : tablica�lad�w)
		{
			window->draw(&(�lad->at(0)), �lad->size(), sf::LineStrip); //std::list<Vertex> - nie dzia�a
		}

		window->display();
	}

	void t�lady()
	{
		Clock clock;
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
			if ((numerCyklu % 10000) == 0 && DEBUG)
				std::cout << numerCyklu << " cykl �lad�w zaja�: " << clock.restart().asMicroseconds() << " us.\n";

			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	}

	Planeta* dodajPlanet�(
		const double r,
		const Vector2d pos,
		const Vector2d vel,
		const double masa,
		const Color color)
	{
		auto ptr = new Planeta(&m_tablicaObiekt�w, r,
			static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
			masa, color);

		mu_tObiekt�w.lock();
		m_tablicaObiekt�w.push_back(ptr);
		mu_tObiekt�w.unlock();

		return ptr;
	}

	PlanetaSta�a* dodajPlanet�Sta��(
		const double r,
		const Vector2d pos,
		const Vector2d vel,
		const double masa,
		const Color color)
	{
		auto ptr = new PlanetaSta�a(&m_tablicaObiekt�w, r,
			static_cast<Vector2d>(pos), static_cast<Vector2d>(vel),
			masa, color);
		mu_tObiekt�w.lock();
		m_tablicaObiekt�w.push_back(ptr);
		mu_tObiekt�w.unlock();

		return ptr;
	}

	/*void zoomScroll(Event &event)
	{
		View view = window->getView();

		Vector2f kursorCoord = window->mapPixelToCoords(Mouse::getPosition(*window));
		Vector2f �rodek = view.getCenter();

		float zmianaSkali = -G_ZOOM_SENS * event.mouseWheelScroll.x;

		if (zmianaSkali > G_ZOOM_MAXSPEED)
			zmianaSkali = G_ZOOM_MAXSPEED;

		if (event.mouseWheelScroll.delta < 0)
			zmianaSkali = -zmianaSkali;

		�rodek -= zmianaSkali * (kursorCoord - �rodek);

		view.zoom(1.0f + zmianaSkali);
		view.setCenter(�rodek);
		window->setView(view);

	}*/

	void usunObiektKursor()
	{
		Vector2i pocz�tek = Mouse::getPosition(*window);

		Planeta *wskazany = nullptr;

		for (Planeta *obiekt : m_tablicaObiekt�w)
		{
			Vector2i pozycjaNaEkranie = window->mapCoordsToPixel(obiekt->getPosition());
			int odle = odl2(pocz�tek, pozycjaNaEkranie);

			if (odle <= obiekt->getRadius())
			{
				wskazany = obiekt;
				break;
			}
		}

		delete wskazany;

	}

	void przesu�WidokOkna(Vector2f a)
	{
		View view = window->getView();
		view.move(a * view.getSize().x / 160.0f);
		window->setView(view);
	}

	void wybierzRozmiar(int i)
	{
		m_wybranaWielkos� += i;
		std::cout << "Wybrano mas� planety " << pow(10.0, m_wybranaWielkos�) << " kg.\n";
	}

	void wybierzPr�dko��Symulacji(double i)
	{
		m_pr�dko��Symulacji *= i;
		std::cout << "Wybrano pr�dko�� symulacji " << m_pr�dko��Symulacji << "x.\n";
	}

	void narysujObiekt()
	{
		Vector2f pocz�tek = window->mapPixelToCoords(Mouse::getPosition(*window));
		while (Mouse::isButtonPressed(Mouse::Left))
		{
		}
		Vector2f koniec = window->mapPixelToCoords(Mouse::getPosition(*window));
		Vector2f pr�dko�� = { (koniec.x - pocz�tek.x)*1.0f, (koniec.y - pocz�tek.y)*1.0f };

		double masa = pow(10.0, G_WybranaWielkos�);
		double �rednica = cbrt(masa);

		Planeta* ptr = dodajPlanet�(�rednica, static_cast<Vector2d>(pocz�tek), static_cast<Vector2d>(pr�dko��), masa, Color(rand(0, 255), rand(0, 255), rand(0, 255)));

		if (DEBUG)
			std::cout << "Narysowano obiekt w adresie: " << ptr << " o predkosci " << pr�dko��.x << " " << pr�dko��.y << "\n";
	}

	void zoomScroll(Event &event)
	{
		View view = window->getView();

		Vector2f kursorCoord = window->mapPixelToCoords(Mouse::getPosition(*window));
		Vector2f �rodek = view.getCenter();

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
			case Event::Resized:	//Zmie� rozmiar okna
				G_WINWIDTH = event.size.width;
				G_WINHEIGHT = event.size.height;
				std::cout << "Nowe wymiary okna: " << event.size.width << "x" << event.size.height << "\n";
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::Up:
					przesu�WidokOkna(Vector2f(0, -10));
					break;
				case Keyboard::Down:
					przesu�WidokOkna(Vector2f(0, 10));
					break;
				case Keyboard::Left:
					przesu�WidokOkna(Vector2f(-10, 0));
					break;
				case Keyboard::Right:
					przesu�WidokOkna(Vector2f(10, 0));
					break;
				case Keyboard::Subtract:
					wybierzRozmiar(-1);
					break;
				case Keyboard::Add:
					wybierzRozmiar(1);
					break;
				case Keyboard::Comma:
					wybierzPr�dko��Symulacji(0.5);
					break;
				case Keyboard::Period:
					wybierzPr�dko��Symulacji(2.0);
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