#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

typedef struct
{
    char nomJoueur[31];
    int score;
} scoreJoueur;

void verifier_position_score(scoreJoueur* tab, int *nbScore, scoreJoueur scoreAenregistrer);
void enregistrer_topscore(int score, char *nomLaby);
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe);

void deplacerAdroite(char *laby, size_t nbLignes, size_t nbColonnes, int *score);
void deplacerAgauche(char *laby, size_t nbLignes, size_t nbColonnes, int *score);
void deplacerEnHaut(char *laby, size_t nbLignes, size_t nbColonnes, int *score);
void deplacerEnBas(char *laby, size_t nbLignes, size_t nbColonnes, int *score);

int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y);
int verifier_bonus_malus(char *laby, size_t nbColonnes, int x, int y);
int verifier_position(size_t nbLignes, size_t nbColonnes);


#endif // JEU_H_INCLUDED
