#pragma once


extern	int			G_WINWIDTH;
extern	int			G_WINHEIGHT;
extern	int			G_WybranaWielkos�;
extern	double		G_Pr�dko��Symulacji;
extern	std::mutex	mu_tObiekt�w;
extern	std::vector	<std::vector<sf::Vertex>*> tablica�lad�w;
extern	std::mutex	mu_t�lad�w;
extern	std::mutex	mu_tRys;