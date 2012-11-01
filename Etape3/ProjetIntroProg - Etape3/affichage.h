#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED

#define TAILLE_LIGNE 10
#include "jeu.h"

void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int score, char *nomLaby, Monstre *tabMonstres, int tailleTabMonstres);
int afficher_accueil();
void afficher_menu();

#endif // AFFICHAGE_H_INCLUDED
