#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

typedef struct
{
    char nomJoueur[31];
    int score;
} scoreJoueur;

typedef struct
{
    int positionX;
    int positionY;
    char typeMonstre;
    char dernierCaractere;
    int penalite;
} Monstre;

int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y);
int verifier_bonus_malus(char *laby, size_t nbColonnes, int x, int y);
int verifier_position_gagnante(size_t nbLignes, size_t nbColonnes);

void deplacer(char *laby, size_t nbLignes, size_t nbColonnes, int directionX, int directionY, int *score, char *nomLabyrinthe, Monstre *tabMonstres, int tailleTabM);
void deplacer_monstres(Monstre *tabMonstres, size_t nbL, size_t nbC, int tailleTabM, char *laby);

Coordonnees *recuperer_murs(char *laby, size_t nbLignes, size_t nbColonnes, int *tailleTabMurs);
Monstre *analyserMonstres(char *laby, size_t nbLignes, size_t nbColonnes, int *tailleTabM);
void remettre_murs(char *laby, size_t nbColonnes, Coordonnees *lesMurs, int tailleTabMurs);
void trouver_chemin_de_sortie(char *laby, size_t nbLignes, size_t nbColonnes);
void verifier_position_score(scoreJoueur* tab, int *nbScore, scoreJoueur scoreAenregistrer);
void enregistrer_topscore(int score, char *nomLaby);
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe);

#endif // JEU_H_INCLUDED
