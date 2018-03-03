#pragma once


#include "Biblioteki.h"

class Tekst : public sf::Text
{
private:

	sf::Font m_font;

public:

	Tekst(std::string fontPath);

	Tekst(const unsigned char fontHex[], const unsigned int length);

	void zmie�Tekst(sf::String & string);
	void skalujTekst(sf::RenderWindow* window, sf::Vector2f zadanaPozycja, sf::Vector2f zadaneProporcja, bool sta�y_rozmiar = 1, bool wyr�wnaj_skale = 0);

};