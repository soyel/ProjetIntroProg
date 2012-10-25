#ifndef GENERATION_H_INCLUDED
#define GENERATION_H_INCLUDED

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
void creer_labyrinthe();

#endif // GENERATION_H_INCLUDED
