#pragma once

#include "Biblioteki.h"


class GUI
{
private:

	sf::RenderWindow		*m_window;
	sf::Text				tCzas;
	sf::Font				fConsolas;

public:

	GUI(sf::RenderWindow * window);

	void drawCzas(sf::RenderWindow & window);

	void odœwie¿Czas(unsigned long long tick, double m_prêdkoœæSymulacji);

};