#include "GUI_text.h"
#include "Sta³e.h"

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
void Tekst::skalujTekst(sf::RenderWindow* window, sf::Vector2f zadanaPozycja, sf::Vector2f zadaneProporcja, bool sta³y_rozmiar, bool wyrównaj_skale)		//TODO Przenieœæ do klasy Tekst
{
	sf::View view = window->getView();
	sf::Vector2f viewSize = view.getSize();
	sf::FloatRect localBounds = getLocalBounds();
	sf::Vector2f zadanaWielkoœæ = { viewSize.x * zadaneProporcja.x, viewSize.y * zadaneProporcja.y };
	sf::Vector2f skala;


	if (sta³y_rozmiar)
	{
		//Sta³y rozmiar niezale¿enie od skali widoku i okna
		sf::Vector2i windowSize = static_cast<sf::Vector2i>(window->getSize());
		skala = { zadanaWielkoœæ.x / localBounds.width / windowSize.x * G_WINWIDTH, zadanaWielkoœæ.y / localBounds.height / windowSize.y * G_WINHEIGHT };	//Czy da sie to zrobiæ bez G_WINxxx?
	}
	else
	{
		//Sta³y rozmiar niezale¿enie od skali widoku
		skala = { zadanaWielkoœæ.x / localBounds.width, zadanaWielkoœæ.y / localBounds.height };
	}

	if (wyrównaj_skale)
		skala.y = skala.x;

	setScale(skala);
	sf::Vector2f œrodekWidoku = view.getCenter();
	sf::Vector2f pozycja = { (zadanaPozycja.x - 0.5f)*viewSize.x + œrodekWidoku.x, (zadanaPozycja.y - 0.5f)*viewSize.y + œrodekWidoku.y, };
	setPosition(pozycja);
}

