#ifndef GENERATION_H_INCLUDED
#define GENERATION_H_INCLUDED

#include "diff_types.h"

chemin initialiserChemin(int id, int x, int y);
void ajouterEnfinDeListe(chemin lesChemins, cellule* leChemin);
void creer_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes);

#endif
