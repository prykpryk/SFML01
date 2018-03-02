#pragma once

#include "Biblioteki.h"
#include "GUI.h"

GUI::GUI(sf::RenderWindow * window) : m_window{ window }
{
	if (!fConsolas.loadFromFile("consola.ttf"))
	{
		// error...
	}

	tCzas.setFont(fConsolas);
	tCzas.setString("XDXDXD");
	tCzas.setCharacterSize(72);
	tCzas.setFillColor(sf::Color::White);
	tCzas.setStyle(sf::Text::Regular);
	tCzas.setScale(0.2f, 0.2f);
	tCzas.setPosition(100, 0);

};

void GUI::drawCzas(sf::RenderWindow & window)
{
	window.draw(tCzas);
}

void GUI::odœwie¿Czas(unsigned long long tick, double prêdkoœæSymulacji)
{
	std::wstring czas = L"Cykli: ";
	czas.append(std::to_wstring(tick));
	czas.append(L"\nKrok czasu: ");
	czas.append(std::to_wstring(prêdkoœæSymulacji/1000.0));
	czas.append(L" ms");

	sf::View view = m_window->getView();

	tCzas.setPosition(m_window->mapPixelToCoords({ 0,0 }, view));
	tCzas.setString(czas);
	tCzas.setScale(sf::Vector2f(1.5f, 1.5f) * m_window->getView().getSize().x / 4000.0f);
}

