#include "stdafx.h"
#include "mutex"
#include <vector>
#include "Definicje.h"
#include "Biblioteki.h"

int			G_WINWIDTH{ 800 };
int			G_WINHEIGHT{ 600 };
std::mutex	mu_tObiekt�w;
std::mutex	mu_t�lad�w;
std::vector	<std::vector<Vertex>*> tablica�lad�w;
