#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include "differents_types.h"

typedef struct
{
    char nomJoueur[31];
    int score;
} scoreJoueur;

int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y);
int verifier_bonus_malus(char *laby, size_t nbColonnes, int x, int y);
int verifier_position_gagnante(size_t nbLignes, size_t nbColonnes);

void deplacer(char *laby, size_t nbLignes, size_t nbColonnes, int directionX, int directionY, int *score, char *nomLabyrinthe, Monstre *tabMonstres, int tailleTabM, SDL_Surface *ecran, InterfaceCreationLaby iCreation, int temps);
void deplacer_monstres(Monstre *tabMonstres, size_t nbL, size_t nbC, int tailleTabM, char *laby);

Monstre *analyserMonstres(char *laby, size_t nbLignes, size_t nbColonnes, int *tailleTabM);
char rechercher_monstre(int x, int y, Monstre *tabMonstres, int tailleTabMonstre);
void trouver_chemin_de_sortie(char *laby, size_t nbLignes, size_t nbColonnes, SDL_Surface *ecran, InterfaceCreationLaby iCreation);
void verifier_position_score(scoreJoueur* tab, int *nbScore, scoreJoueur scoreAenregistrer, SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby);
void enregistrer_topscore(int scoreTotal, char *nomLaby, SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby, char *laby, size_t nbLignes, size_t nbColonnes, Monstre *tabMonstres, int tailleTabMonstres, int temps, int score);
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe, SDL_Surface *ecran, InterfaceCreationLaby iCreation);
Saisie recuperer_nom_score(SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby);

#endif // JEU_H_INCLUDED
