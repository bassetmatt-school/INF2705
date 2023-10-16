#ifndef UTILS_H
#define UTILS_H

double rand01();

void getGroupRandomPos(int groupId, const int N_ROWS, float& x, float& z);

void getGrassRandomPosTex(float& x, float& z, int& texId);

// Votre main loop devrait dessiner dans un ordre semblable:

// Ruisseau   - partie 2
// Gazon      - partie 3
// Personnage (si besoin)
// Sol
// Modèles (autant que possible tous ensembles, tous les arbres, tous les rochers, tous les champignons)
// HUD (carré/coeur dans l'écran)
// Skybox - partie 2


#endif // UTILS_H
