#pragma once

#include "Biblioteki.h"
#include "GUI_text.h"

class GUI : public sf::Drawable
{
private:

	sf::RenderWindow		*m_window;
	sf::Font				fConsolas;
	std::vector<std::shared_ptr<sf::Drawable>> m_rysowalneGUI;

	std::shared_ptr<Tekst>	m_czas;

public:

	GUI(sf::RenderWindow * window);

	void odœwie¿Czas(unsigned long long tick, double m_prêdkoœæSymulacji, int liczba_planet);

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	std::shared_ptr<Tekst> dodajTekst(std::string fontPath);

};