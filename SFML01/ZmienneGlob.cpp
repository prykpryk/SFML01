//#include "stdafx.h"
#include "mutex"
#include <vector>
#include "Definicje.h"
#include "Biblioteki.h"

int			G_WINWIDTH{ 800 };
int			G_WINHEIGHT{ 600 };
int			G_WybranaWielkos�{ 1 };
double		G_Pr�dko��Symulacji{ 1.0 };
std::mutex	mu_tObiekt�w;
std::mutex	mu_t�lad�w;
std::mutex	mu_tRys;
std::vector	<std::vector<sf::Vertex>*> tablica�lad�w;
