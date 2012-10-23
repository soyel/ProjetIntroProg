#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED


void jouer(char *laby, size_t nbLignes, size_t nbColonnes);

void deplacerAdroite(char *laby, size_t nbLignes, size_t nbColonnes, int *score);
void deplacerAgauche(char *laby, size_t nbLignes, size_t nbColonnes, int *score);
void deplacerEnHaut(char *laby, size_t nbLignes, size_t nbColonnes, int *score);
void deplacerEnBas(char *laby, size_t nbLignes, size_t nbColonnes, int *score);

int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y);
int verifier_bonus_malus(char *laby, size_t nbColonnes, int x, int y);
int verifier_position(size_t nbLignes, size_t nbColonnes);

#endif // JEU_H_INCLUDED
