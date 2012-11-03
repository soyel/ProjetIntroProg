#include "tools.h"

void initSaisie(Saisie *saisie)
{
    saisie->enSaisie = 1;  // pour dire que l'on est en train de saisir
    saisie->indice = 0; // on en est au premier caractère.
    saisie->valide = 0;
}

void saisirCaractere(Saisie *saisie, char caractere, int positionEntree)
{
    int index = saisie->indice;
    if(positionEntree == 1)
    {
        if(index < 31)
        {
            saisie->nomL[saisie->indice] = caractere;
            saisie->indice = saisie->indice + 1;
        }
    }
    if(positionEntree == 2)
    {
        if(index < 2 && isdigit(caractere) )
        {
            saisie->nbLignes[saisie->indice] = caractere;
            saisie->indice = saisie->indice + 1;
        }
    }
    if(positionEntree == 3)
    {
        if(index < 2 && isdigit(caractere))
        {
            saisie->nbColonnes[saisie->indice] = caractere;
            saisie->indice = saisie->indice + 1;
        }
    }

}

void effacerCaractere(Saisie *saisie)
{
    if(saisie->indice != 0)
    {
        saisie->indice = saisie->indice - 1;
    }
}
void finSaisie(Saisie *saisie, int *positionEntree, SDL_Rect *positionIndex)
{
    int chiffre;
    int positionE = *positionEntree;
    if(saisie->indice != 0)
    {
        if(positionE == 1)
        {

            saisie->nomL[saisie->indice] = 0; // le 0 terminal
            *positionEntree += 1;
            positionIndex->y += 80;
        }
        if(positionE == 2)
        {
            chiffre = atoi(saisie->nbLignes);
            if(chiffre >= 5 && chiffre % 2 != 0)
            {
                saisie->nbLignes[saisie->indice] = 0; // le 0 terminal
                *positionEntree += 1;
                positionIndex->y += 80;
            }
        }
        if(positionE == 3)
        {
            chiffre = atoi(saisie->nbLignes);
            if(chiffre >= 5 && chiffre % 2 != 0)
            {
                saisie->nbLignes[saisie->indice] = 0; // le 0 terminal
                *positionEntree += 1;
                positionIndex->y += 80;
            }
        }
        saisie->indice = 0;
    }
}

char *recuperer_partie_texte(Saisie saisie, int positionEntree)
{
    int tailleSaisie = saisie.indice;
    char *chaineTemporaire = NULL;
    chaineTemporaire = malloc(sizeof(int) * tailleSaisie);

    int i;

    for(i = 0; i < tailleSaisie; i++)
    {
        if(positionEntree == 1) chaineTemporaire[i] = saisie.nomL[i];
        if(positionEntree == 2) chaineTemporaire[i] = saisie.nbLignes[i];
        if(positionEntree == 3) chaineTemporaire[i] = saisie.nbColonnes[i];

    }
    chaineTemporaire[i] = 0;
    return chaineTemporaire;
}
