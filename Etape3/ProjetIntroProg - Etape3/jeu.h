#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include "diff_types.h"

int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y);
int verifier_bonus_malus(char *laby, size_t nbColonnes, int x, int y);
int verifier_position_gagnante(size_t nbLignes, size_t nbColonnes);

void deplacer(char *laby, size_t nbLignes, size_t nbColonnes, int directionX, int directionY, int *score, char *nomLabyrinthe, Monstre *tabMonstres, int tailleTabM, int temps);
void deplacer_monstres(Monstre *tabMonstres, size_t nbL, size_t nbC, int tailleTabM, char *laby);

Monstre *analyserMonstres(char *laby, size_t nbLignes, size_t nbColonnes, int *tailleTabM);
char rechercher_monstre(int x, int y, Monstre *tabMonstres, int tailleTabMonstre);
void trouver_chemin_de_sortie(char *laby, size_t nbLignes, size_t nbColonnes);
void verifier_position_score(scoreJoueur* tab, int *nbScore, scoreJoueur scoreAenregistrer);
void enregistrer_topscore(int score, char *nomLaby);
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe);

#endif
