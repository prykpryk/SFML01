#pragma once

#include "Definicje.h"
#include "Biblioteki.h"
#include "GUI.h"
#include "GUI_text.h"
#include "Sta³e.h"

#include "font_tahoma.h"

GUI::GUI(sf::RenderWindow * window) : m_window{ window }
{
	//m_czas = dodajTekst("tahoma.ttf");
	m_czas = dodajTekst(tahoma_ttf, tahoma_ttf_len);
};


void GUI::odœwie¿Czas(unsigned long long tick, double prêdkoœæSymulacji, int liczba_planet)
{
	//Okreœlanie tekstu, fps
	static sf::Clock clock;
	static long long ostatni_tick = -1;
	static int fps = 0;
	if (clock.getElapsedTime().asSeconds() > 1.0f)
	{
		fps = static_cast<int>(floor((tick - ostatni_tick) / clock.restart().asSeconds()));
		ostatni_tick = tick;
	}
	std::wstring czas = L"Fiz FPS: ";
	czas.append(std::to_wstring(fps));
	czas.append(L"\nKrok czasu: ");
	czas.append(std::to_wstring(prêdkoœæSymulacji * G_PodstawaCzasu * 1000));
	czas.append(L" ms");
	if (DEBUG)
	{
		czas.append(L"\nIloœæ wskaŸników na zegar: ");
		czas.append(std::to_wstring(m_czas.use_count()));
	}
	czas.append(L"\nLiczba planet: ");
	czas.append(std::to_wstring(liczba_planet));
	m_czas->setString(czas);

	//Skalowanie tekstu
	m_czas->skalujTekst(m_window, { 0.0f ,0.0f }, {0.2f, 0.2f}, true, true);

}


void GUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto i : m_rysowalneGUI)
	{
		target.draw(*i);
	}
}

std::shared_ptr<Tekst> GUI::dodajTekst(std::string fontPath)
{
	std::shared_ptr<Tekst> ptr = std::make_shared<Tekst>(fontPath);
	m_rysowalneGUI.push_back(ptr);
	return ptr;
}

std::shared_ptr<Tekst> GUI::dodajTekst(const unsigned char fontHex[], const unsigned int length)
{
	std::shared_ptr<Tekst> ptr = std::make_shared<Tekst>(fontHex, length);
	m_rysowalneGUI.push_back(ptr);
	return ptr;
}
