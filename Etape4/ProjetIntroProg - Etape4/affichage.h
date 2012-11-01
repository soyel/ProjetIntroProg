#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED

#define TAILLE_LIGNE 10

#include "differents_types.h"

void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int score, char *nomLaby, Monstre *tabMonstres, int tailleTabMonstres, SDL_Surface *ecran, InterfaceCreationLaby iCreation);
int afficher_accueil();
void afficher_menu();

#endif // AFFICHAGE_H_INCLUDED
