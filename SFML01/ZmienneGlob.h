#pragma once


extern	int			G_WINWIDTH;
extern	int			G_WINHEIGHT;
extern	std::mutex	mu_tObiektów;
extern	std::vector	<std::vector<sf::Vertex>*> tablicaŒladów;
extern	std::mutex	mu_tŒladów;
extern	std::mutex	mu_tRys;