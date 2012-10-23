#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "jeu.h"
#include "affichage.h"

int positionActuelle_X = 1;
int positionActuelle_Y = 0;


int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y){
    if(laby[nbColonnes * (positionActuelle_X + x) + (positionActuelle_Y + y)]!='#'){
        return 1;
    }
    return 0;
}

int verifier_bonus_malus(char *laby, size_t nbColonnes, int x, int y)
{
    if(laby[nbColonnes * (positionActuelle_X + x) + (positionActuelle_Y + y)] == 'b')
    {
        return 1;
    }
    else if(laby[nbColonnes * (positionActuelle_X + x) + (positionActuelle_Y + y)] == 'm')
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
void deplacerAdroite(char *laby, size_t nbLignes, size_t nbColonnes, int *score){
    if(verifier_possibilite(laby, nbColonnes, 0, 1) == 1){
        *score += 10 * verifier_bonus_malus(laby, nbColonnes, 0, 1);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_Y +=1;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}
void deplacerAgauche(char *laby, size_t nbLignes, size_t nbColonnes, int *score){
    if(verifier_possibilite(laby, nbColonnes, 0, -1) == 1){
        *score += 10 * verifier_bonus_malus(laby, nbColonnes, 0, -1);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_Y -= 1;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}
void deplacerEnHaut(char *laby, size_t nbLignes, size_t nbColonnes, int *score){
    if(verifier_possibilite(laby, nbColonnes, -1, 0) == 1){
        *score +=  10 * verifier_bonus_malus(laby, nbColonnes, -1, 0);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_X -= 1;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}
void deplacerEnBas(char *laby, size_t nbLignes, size_t nbColonnes, int *score){
    if(verifier_possibilite(laby, nbColonnes, 1, 0) == 1){
        *score += 10 * verifier_bonus_malus(laby, nbColonnes, 1, 0);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_X += 1;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}

int verifier_position(size_t nbLignes, size_t nbColonnes){
    if(positionActuelle_X == nbLignes - 2 && positionActuelle_Y == nbColonnes - 1){
        return 1;
    }
    return 0;
}

void jouer(char *laby, size_t nbLignes, size_t nbColonnes){
    system("CLS");
    laby[nbColonnes * 1 + 0] = 'o';
    int score = 0;
    int *p_score = &score;
    afficher_labyrinthe(laby, nbLignes, nbColonnes, score);
    char touche = 'l';
    fflush(stdin);
    clock_t temps;
    while(touche != 'f'){
        touche = getch();
        switch(touche){
            case 'd': deplacerAdroite(laby, nbLignes, nbColonnes, p_score);
                        break;
            case 'q': deplacerAgauche(laby, nbLignes, nbColonnes, p_score);
                        break;
            case 's': deplacerEnBas(laby, nbLignes, nbColonnes, p_score);
                        break;
            case 'z': deplacerEnHaut(laby, nbLignes, nbColonnes, p_score);
                        break;
        }
        if(verifier_position(nbLignes, nbColonnes)==1){
            temps = clock();
            touche = 'f';
        }
    }
    int tempsPartie = (int) temps/CLOCKS_PER_SEC;
    _getch();
    positionActuelle_X = 1;
    positionActuelle_Y = 0;
    laby[nbColonnes * (nbLignes-2) + (nbColonnes - 1)] = ' ';
    return;
}
