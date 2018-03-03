#pragma once


#include "Biblioteki.h"

class Tekst : public sf::Text
{
private:

	sf::Font m_font;

public:

	Tekst(std::string fontPath);

	Tekst(const unsigned char fontHex[], const unsigned int length);

	void zmieñTekst(sf::String & string);
	void skalujTekst(sf::RenderWindow* window, sf::Vector2f zadanaPozycja, sf::Vector2f zadaneProporcja, bool sta³y_rozmiar = 1, bool wyrównaj_skale = 0);

};