#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED

#define TAILLE_LIGNE 10

#include "diff_types.h"

void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int score, char *nomLaby, Monstre *tabMonstres, int tailleTabMonstres, int temps);
int afficher_accueil();
void afficher_menu();
void menu_creation(FILE *fichier, char *nomFichier, char *nomLaby, int *fichierCharge);
void menu_chargement(FILE *fichier, char *nomFichier, char *nomLaby, int *fichierCharge);
void menu_jouer(FILE *fichier, char *nomFichier, char *nomLaby, int *fichierCharge, int *continuerAff);
void menu_afficher_solution(FILE *fichier, char *nomFichier, int *fichierCharge);

#endif
