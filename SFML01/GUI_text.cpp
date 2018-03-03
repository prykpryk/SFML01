#include "GUI_text.h"

Tekst::Tekst(std::string fontPath)
{
	if (!m_font.loadFromFile(fontPath))
	{
		throw L"Nie uda³o siê za³adowaæ pliku czcionki";
	}

	setFont(m_font);

	setString("test");
	setCharacterSize(72);
	setFillColor(sf::Color::White);
	setStyle(sf::Text::Regular);
	setScale(0.2f, 0.2f);
	setPosition(100, 0);

}

void Tekst::zmieñTekst(sf::String & string)
{
	setString(string);
}


//Zadana pozycja i zadana proporcja to wielokrotnoœæ rozmiaru okna.
void Tekst::skalujTekst(sf::RenderWindow* window, sf::Vector2f zadanaPozycja, sf::Vector2f zadaneProporcja)		//TODO Przenieœæ do klasy Tekst
{
	sf::View view = window->getView();
	//Sta³y rozmiar niezale¿enie od skali widoku i okna
	sf::FloatRect localBounds = getLocalBounds();

	sf::Vector2f viewSize = view.getSize();

	sf::Vector2f zadanaWielkoœæ = { viewSize.x * zadaneProporcja.x, viewSize.y * zadaneProporcja.y };
	sf::Vector2f skala = { zadanaWielkoœæ.x / localBounds.width, zadanaWielkoœæ.y / localBounds.height };
	setScale(skala);

	sf::Vector2f œrodekWidoku = view.getCenter();
	sf::Vector2f pozycja = { (zadanaPozycja.x - 0.5f)*viewSize.x + œrodekWidoku.x, (zadanaPozycja.y - 0.5f)*viewSize.y + œrodekWidoku.y, };
	setPosition(pozycja);
}

