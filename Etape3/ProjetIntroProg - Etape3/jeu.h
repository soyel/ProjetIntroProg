#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

typedef struct
{
    char nomJoueur[31];
    int score;
} scoreJoueur;

int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y);
int verifier_bonus_malus(char *laby, size_t nbColonnes, int x, int y);
int verifier_position_gagnante(size_t nbLignes, size_t nbColonnes);

void deplacer(char *laby, size_t nbLignes, size_t nbColonnes, int directionX, int directionY, int *score, char *nomLabyrinthe);

void trouver_chemin_de_sortie(char *laby, size_t nbLignes, size_t nbColonnes);
void verifier_position_score(scoreJoueur* tab, int *nbScore, scoreJoueur scoreAenregistrer);
void enregistrer_topscore(int score, char *nomLaby);
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe, int avecClassement);

#endif // JEU_H_INCLUDED
