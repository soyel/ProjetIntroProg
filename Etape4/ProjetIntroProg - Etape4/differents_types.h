#ifndef DIFFERENTS_TYPES_H_INCLUDED
#define DIFFERENTS_TYPES_H_INCLUDED

typedef struct
{
    int positionX;
    int positionY;
    char typeMonstre;
    int penalite;
} Monstre;

typedef struct
{
    SDL_Surface *imageDeFond2;
    SDL_Surface *retourMenu;
    SDL_Surface *labelNomLaby;
    SDL_Surface *labelNbLignes;
    SDL_Surface *labelNbColonnes;
    SDL_Surface *pointeurEntree;
    SDL_Surface *labelScore;
    SDL_Surface *malusTemps;
    SDL_Surface *scoreT;
    SDL_Surface *classementBar;
    SDL_Rect positionImageDeFond2;
    SDL_Rect positionRetourMenu;
    SDL_Rect positionLabelNomLaby;
    SDL_Rect positionLabelNbLignes;
    SDL_Rect positionLabelNbColonnes;
    SDL_Rect positionPointeurEntree;
    SDL_Rect positionLabelScore;
    SDL_Rect positionMalusTemps;
    SDL_Rect positionScoreTotal;
    SDL_Rect positionClassementBar;
} InterfaceCreationLaby;

typedef struct
{
    char nomL[31]; // on autorise un nom de 23 caractère
    char nbLignes[3];
    char nbColonnes[3];
    int enSaisie;
    int indice;
    int valide;
} Saisie;

#endif // DIFFERENTS_TYPES_H_INCLUDED
