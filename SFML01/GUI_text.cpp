#include "GUI_text.h"

Tekst::Tekst(std::string fontPath)
{
	if (!m_font.loadFromFile(fontPath))
	{
		throw L"Nie uda�o si� za�adowa� pliku czcionki";
	}

	setFont(m_font);

	setString("test");
	setCharacterSize(72);
	setFillColor(sf::Color::White);
	setStyle(sf::Text::Regular);
	setScale(0.2f, 0.2f);
	setPosition(100, 0);

}

void Tekst::zmie�Tekst(sf::String & string)
{
	setString(string);
}


//Zadana pozycja i zadana proporcja to wielokrotno�� rozmiaru okna.
void Tekst::skalujTekst(sf::RenderWindow* window, sf::Vector2f zadanaPozycja, sf::Vector2f zadaneProporcja)		//TODO Przenie�� do klasy Tekst
{
	sf::View view = window->getView();
	//Sta�y rozmiar niezale�enie od skali widoku i okna
	sf::FloatRect localBounds = getLocalBounds();

	sf::Vector2f viewSize = view.getSize();

	sf::Vector2f zadanaWielko�� = { viewSize.x * zadaneProporcja.x, viewSize.y * zadaneProporcja.y };
	sf::Vector2f skala = { zadanaWielko��.x / localBounds.width, zadanaWielko��.y / localBounds.height };
	setScale(skala);

	sf::Vector2f �rodekWidoku = view.getCenter();
	sf::Vector2f pozycja = { (zadanaPozycja.x - 0.5f)*viewSize.x + �rodekWidoku.x, (zadanaPozycja.y - 0.5f)*viewSize.y + �rodekWidoku.y, };
	setPosition(pozycja);
}

