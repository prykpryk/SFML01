#pragma once


#include "Biblioteki.h"

class Tekst : public sf::Text
{
private:

	sf::Font m_font;

public:

	Tekst(std::string fontPath);

	void zmie�Tekst(sf::String & string);
	void skalujTekst(sf::RenderWindow * window, sf::Vector2f zadanaPozycja, sf::Vector2f zadaneProporcja);
};