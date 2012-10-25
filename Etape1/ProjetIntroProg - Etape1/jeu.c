#incldue <stdlib.h>
#include <stdio.h>

#include "jeu.h"

int positionActuelle_X = 1;
int positionActuelle_Y = 0;

/**
 * Vérifie que la diréction dans laquelle on veut se déplacer n'est pas un mur
 * @param x abscisse de la direction
 * @param y ordonnée de la direction
 * @return 0 si la direction est un mur, 0 sinon
 */
int verifier_possibilite(int x, int y)
{
    if(laby[positionActuelle_X + x][positionActuelle_Y + y]!='#')
    {
        return 1;
    }
    return 0;
}

/**
 * Déplace le joueur à droite de sa position actuelle,
 * à condition que cette direction ne soit pas un mur
 */
void deplacerAdroite()
{
    if(verifier_possibilite(0, 1) == 1)
    {
        laby[positionActuelle_X][positionActuelle_Y] = ' ';
        positionActuelle_Y +=1;
        laby[positionActuelle_X][positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinth();
    }
    return;
}

/**
 * Déplace le joueur à gauche de sa position actuelle,
 * à condition que cette direction ne soit pas un mur
 */
void deplacerAgauche()
{
    if(verifier_possibilite(0, -1) == 1)
    {
        laby[positionActuelle_X][positionActuelle_Y] = ' ';
        positionActuelle_Y -= 1;
        laby[positionActuelle_X][positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinth();
    }
    return;
}

/**
 * Déplace le joueur en haut de sa position actuelle,
 * à condition que cette direction ne soit pas un mur
 */
void deplacerEnHaut()
{
    if(verifier_possibilite(-1, 0) == 1)
    {
        laby[positionActuelle_X][positionActuelle_Y] = ' ';
        positionActuelle_X -= 1;
        laby[positionActuelle_X][positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinth();
    }
    return;
}

/**
 * Déplace le joueur en bas de sa position actuelle,
 * à condition que cette direction ne soit pas un mur
 */
void deplacerEnBas()
{
    if(verifier_possibilite(1, 0) == 1)
    {
        laby[positionActuelle_X][positionActuelle_Y] = ' ';
        positionActuelle_X += 1;
        laby[positionActuelle_X][positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinth();
    }
    return;
}

/**
 * Vérifie si le joueur est positioné sur la case sortie
 * @return 1 si le joueur est positioné sur la case sortie, 0 sinon
 */
int verifier_position()
{
    if(positionActuelle_X == LIGNES - 2 && positionActuelle_Y == COLONNES - 1)
    {
        return 1;
    }
    return 0;
}

/**
 * Joue une partie jusqu'à ce que le joueur n'atteigne la sortie,
 * ou appuie sur la touche 'f'
 */
void jouer(){
    system("CLS");
    laby[1][0] = 'o';
    afficher_labyrinth();
    char touche = 'l';
    fflush(stdin);
    while(touche != 'f')
    {
        touche = getch();
        switch(touche)
        {
            case 'd': deplacerAdroite();
                        break;
            case 'q': deplacerAgauche();
                        break;
            case 's': deplacerEnBas();
                        break;
            case 'z': deplacerEnHaut();
                        break;
        }
        if(verifier_position()==1)
        {
            touche = 'f';
            positionActuelle_X = 1;
            positionActuelle_Y = 0;
            laby[LIGNES - 2][COLONNES - 1] = ' ';
        }
    }
    return;
}

