#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

void enregistrer_topscore(int score, char *nomLaby)
{
    FILE* fichier;
    char nomFichier[31];
    strcpy(nomFichier, nomLaby);
    strcat(nomFichier, ".cfg");
    printf("nom fichier : %s\n", nomFichier);
    fichier = fopen(nomFichier, "r+");
    int auMoinsUnJoueur = 0;
    int enregistrementNom = 1;
    char ligneLue[50] = "";
    struct scoreJoueur scoreAenregistrer;
    struct scoreJoueur scoreLu;
    scoreAenregistrer.score = score;
    struct scoreJoueur tableauScore[10];
    int nbScore = 0;
    char chaineTemporaire[31];
    char ligneAinserer[51];
    int position_caractere = 0;
    char caractereLu;

    memset(&scoreLu.nomJoueur[0], 0, sizeof(scoreLu.nomJoueur));
    memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));
    if(fichier != NULL)
    {
        //lire premier ligne pour connaitre le nb de score deja enregistre
        while((caractereLu = fgetc(fichier)) != EOF)
        {
            if(caractereLu == '\n'){
                scoreLu.score = atoi(chaineTemporaire);
                tableauScore[nbScore] = scoreLu;
                printf("Joueur %s avec score %d\n", scoreLu.nomJoueur, scoreLu.score);
                memset(&scoreLu.nomJoueur[0], 0, sizeof(scoreLu.nomJoueur));
                memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));
                enregistrementNom = 1;
                position_caractere = 0;
                nbScore += 1;
            }
            else
            {
                if(caractereLu == ':')
                {
                    position_caractere = 0;
                    enregistrementNom = 0;
                }
                else
                {

                    if(enregistrementNom == 1)
                    {
                        scoreLu.nomJoueur[position_caractere] = caractereLu;
                        position_caractere += 1;
                        printf("Nom :%s\n", scoreLu.nomJoueur);

                    }
                    else
                    {
                        chaineTemporaire[position_caractere] = caractereLu;
                        position_caractere += 1;
                        printf("Score :%s\n", chaineTemporaire);
                    }
                }
            }

            /*strcpy(scoreLu.nomJoueur, ligneLue);
            fgets(ligneLue, 50, fichier);
            scoreLu.score = atoi(ligneLue);*/
            auMoinsUnJoueur = 1;

            //printf("Joueur %s avec score %d\n", scoreLu.nomJoueur, scoreLu.score);
        } //Lire tant qu'on atteint pas la fin du fichier, ligne par ligne
        //chaque ligne contient un score
        scoreLu.score = atoi(chaineTemporaire);
        tableauScore[nbScore] = scoreLu;
        printf("Joueur %s avec score %d\n", scoreLu.nomJoueur, scoreLu.score);
        memset(&scoreLu.nomJoueur[0], 0, sizeof(scoreLu.nomJoueur));
        memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));
        enregistrementNom = 1;
        position_caractere = 0;
        nbScore += 1;
        if(auMoinsUnJoueur == 0){
            printf("Bravo! Votre score va etre enregistre !\n");
            printf("Entrez votre nom : ");
            scanf("%s", scoreAenregistrer.nomJoueur);
            printf("Ce qui va etre enregistre : %s:%d", scoreAenregistrer.nomJoueur, scoreAenregistrer.score);
            itoa(scoreAenregistrer.score, chaineTemporaire, 10);
            strcpy(ligneAinserer, scoreAenregistrer.nomJoueur);
            strcat(ligneAinserer, ":");
            strcat(ligneAinserer, chaineTemporaire);
            fputs(ligneAinserer, fichier);
            /*fputs(scoreAenregistrer.nomJoueur, fichier);
            fputs("\o", fichier);
            fputs(chaineTemporaire, fichier);*/
        }
        else
        {
            if(nbScore == 10)
            {
                printf("haha\n");
            }
            else
            {
                printf("Bravo! Votre score va etre enregistre !\n");
                printf("Entrez votre nom : ");
                scanf("%s", scoreAenregistrer.nomJoueur);
                printf("Ce qui va etre enregistre : %s:%d", scoreAenregistrer.nomJoueur, scoreAenregistrer.score);
                itoa(scoreAenregistrer.score, chaineTemporaire, 10);
                strcpy(ligneAinserer, scoreAenregistrer.nomJoueur);
                strcat(ligneAinserer, ":");
                strcat(ligneAinserer, chaineTemporaire);
                fputs("\n", fichier);
                fputs(ligneAinserer, fichier);
            }
        }


        //faire le trie dans le tableau

        //recharger le fichier avec le bon tableau
        fclose(fichier);
    }
    else
    {
        printf("Impossible d'ouvrir\n");
        _getch();
    }
}
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe){
    system("CLS");
    laby[nbColonnes * 1 + 0] = 'o';
    int score = 0;
    int *p_score = &score;
    afficher_labyrinthe(laby, nbLignes, nbColonnes, score);
    char touche = 'l';
    fflush(stdin);
    clock_t temps;
    time_t debut, fin;
    debut = time(NULL);
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
            //temps = clock();
            fin = time(NULL);
            //int tempsPartie = (int) temps/CLOCKS_PER_SEC;
            printf("tempsPartie : %d", difftime(fin, debut));
            //enregistrer_topscore(score + tempsPartie * 10, nomLabyrinthe);
            touche = 'f';
        }
    }

    _getch();
    positionActuelle_X = 1;
    positionActuelle_Y = 0;
    laby[nbColonnes * (nbLignes-2) + (nbColonnes - 1)] = ' ';
    return;
}
