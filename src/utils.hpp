#ifndef UTILS_H
#define UTILS_H

double rand01();

void getGroupRandomPos(int groupId, const int N_ROWS, float& x, float& z);


// Pour la partie 3 avec les positions aléatoires du gazon:
/*
	float randomX = -25.0f + rand01() * 50.0f;
	float randomZ = -25.0f + rand01() * 50.0f;
	double rval = rand01();
	int randomTexture = rval > 0.05f ? rval > 0.10f ? 0 : 1 : 2; // Numero de texture entre [0,2]
*/


// Votre main loop devrait dessiner dans un ordre semblable:

// Ruisseau   - partie 2
// Gazon      - partie 3
// Personnage (si besoin)
// Sol
// Modèles (autant que possible tous ensembles, tous les arbres, tous les rochers, tous les champignons)
// HUD (carré/coeur dans l'écran)
// Skybox - partie 2


#endif // UTILS_H
