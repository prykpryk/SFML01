//#include "stdafx.h"
#include "mutex"
#include <vector>
#include "Definicje.h"
#include "Biblioteki.h"

int			G_WINWIDTH{ 800 };
int			G_WINHEIGHT{ 600 };
std::mutex	mu_tObiektów;
std::mutex	mu_tŒladów;
std::mutex	mu_tRys;
std::vector	<std::vector<sf::Vertex>*> tablicaŒladów;
