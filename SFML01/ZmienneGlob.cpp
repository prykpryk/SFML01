#include "stdafx.h"
#include "mutex"
#include <vector>
#include "Biblioteki.h"

int			G_WINWIDTH{ 800 };
int			G_WINHEIGHT{ 600 };
std::mutex	mu_tObiekt�w;
std::mutex	mu_t�lad�w;
std::vector	<sf::VertexArray*> tablica�lad�w;
