#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <SDL.h>
#include "differents_types.h"

void initSaisie(Saisie *saisie);
void saisirCaractere(Saisie *saisie, char caractere, int positionEntree);
void finSaisie(Saisie *saisie, int *positionEntree, SDL_Rect *positionIndex);
char *recuperer_partie_texte(Saisie saisie, int positionEntree);
void effacerCaractere(Saisie *saisie);

#endif
