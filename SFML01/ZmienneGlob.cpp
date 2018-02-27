//#include "stdafx.h"
#include "mutex"
#include <vector>
#include "Definicje.h"
#include "Biblioteki.h"

int			G_WINWIDTH{ 800 };
int			G_WINHEIGHT{ 600 };
int			G_WybranaWielkosæ{ 1 };
double		G_PrêdkoœæSymulacji{ 1.0 };
std::mutex	mu_tObiektów;
std::mutex	mu_tŒladów;
std::mutex	mu_tRys;
std::vector	<std::vector<sf::Vertex>*> tablicaŒladów;
