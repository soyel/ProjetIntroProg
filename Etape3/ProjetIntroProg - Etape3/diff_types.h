#ifndef DIFF_TYPES_H_INCLUDED
#define DIFF_TYPES_H_INCLUDED

typedef struct
{
    int x;
    int y;
} Coordonnees;
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
    int penalite;
} Monstre;

#endif
