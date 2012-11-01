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
    SDL_Rect positionImageDeFond2;
    SDL_Rect positionRetourMenu;
    SDL_Rect positionLabelNomLaby;
    SDL_Rect positionLabelNbLignes;
    SDL_Rect positionLabelNbColonnes;
    SDL_Rect positionPointeurEntree;
} InterfaceCreationLaby;

#endif // DIFFERENTS_TYPES_H_INCLUDED
