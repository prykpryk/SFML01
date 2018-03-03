#include "GUI_text.h"
#include "Sta�e.h"

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
void Tekst::skalujTekst(sf::RenderWindow* window, sf::Vector2f zadanaPozycja, sf::Vector2f zadaneProporcja, bool sta�y_rozmiar, bool wyr�wnaj_skale)		//TODO Przenie�� do klasy Tekst
{
	sf::View view = window->getView();
	sf::Vector2f viewSize = view.getSize();
	sf::FloatRect localBounds = getLocalBounds();
	sf::Vector2f zadanaWielko�� = { viewSize.x * zadaneProporcja.x, viewSize.y * zadaneProporcja.y };
	sf::Vector2f skala;


	if (sta�y_rozmiar)
	{
		//Sta�y rozmiar niezale�enie od skali widoku i okna
		sf::Vector2i windowSize = static_cast<sf::Vector2i>(window->getSize());
		skala = { zadanaWielko��.x / localBounds.width / windowSize.x * G_WINWIDTH, zadanaWielko��.y / localBounds.height / windowSize.y * G_WINHEIGHT };	//Czy da sie to zrobi� bez G_WINxxx?
	}
	else
	{
		//Sta�y rozmiar niezale�enie od skali widoku
		skala = { zadanaWielko��.x / localBounds.width, zadanaWielko��.y / localBounds.height };
	}

	if (wyr�wnaj_skale)
		skala.y = skala.x;

	setScale(skala);
	sf::Vector2f �rodekWidoku = view.getCenter();
	sf::Vector2f pozycja = { (zadanaPozycja.x - 0.5f)*viewSize.x + �rodekWidoku.x, (zadanaPozycja.y - 0.5f)*viewSize.y + �rodekWidoku.y, };
	setPosition(pozycja);
}

