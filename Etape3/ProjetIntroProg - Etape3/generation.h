#ifndef GENERATION_H_INCLUDED
#define GENERATION_H_INCLUDED

struct coordonnees
{
    int x;
    int y;
};
struct coordonneesMurs
{
    int x;
    int y;
    char type;
};

typedef struct cellule cellule;
struct cellule
{
    int x;
    int y;
    int identifiant;
    struct cellule *celluleSuivante;
    struct cellule *premiereCellule;
};

typedef cellule* chemin;

chemin initialiserChemin(int id, int x, int y);
void ajouterEnfinDeListe(chemin lesChemins, cellule* leChemin);
void trouver_chemin_de_sortie(char *laby, size_t nbLignes, size_t nbColonnes);
void creer_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes);
void remplir_labyrinthe(FILE *fichier, char *laby, size_t nbLignes, size_t nbColonnes);

#endif // GENERATION_H_INCLUDED