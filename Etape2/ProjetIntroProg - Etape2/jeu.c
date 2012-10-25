#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef __WIN32__
    #define clear() system("cls")
#else
    #define clear() system("clear")
#endif

#include "jeu.h"
#include "affichage.h"

int positionActuelle_X = 1;
int positionActuelle_Y = 0;


int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y)
{
    if(laby[nbColonnes * (positionActuelle_X + x) + (positionActuelle_Y + y)]!='#')
    {
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

void deplacerAdroite(char *laby, size_t nbLignes, size_t nbColonnes, int *score)
{
    if(verifier_possibilite(laby, nbColonnes, 0, 1) == 1){
        *score += 10 * verifier_bonus_malus(laby, nbColonnes, 0, 1);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_Y +=1;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        clear();
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}
void deplacerAgauche(char *laby, size_t nbLignes, size_t nbColonnes, int *score)
{
    if(verifier_possibilite(laby, nbColonnes, 0, -1) == 1)
    {
        *score += 10 * verifier_bonus_malus(laby, nbColonnes, 0, -1);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_Y -= 1;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        clear();
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}
void deplacerEnHaut(char *laby, size_t nbLignes, size_t nbColonnes, int *score)
{
    if(verifier_possibilite(laby, nbColonnes, -1, 0) == 1)
    {
        *score +=  10 * verifier_bonus_malus(laby, nbColonnes, -1, 0);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_X -= 1;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        clear();
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}
void deplacerEnBas(char *laby, size_t nbLignes, size_t nbColonnes, int *score)
{
    if(verifier_possibilite(laby, nbColonnes, 1, 0) == 1)
    {
        *score += 10 * verifier_bonus_malus(laby, nbColonnes, 1, 0);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_X += 1;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        clear();
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}

int verifier_position(size_t nbLignes, size_t nbColonnes)
{
    if(positionActuelle_X == nbLignes - 2 && positionActuelle_Y == nbColonnes - 1){
        return 1;
    }
    return 0;
}

void verifier_position_score(scoreJoueur* tab, int *nbScore, scoreJoueur scoreAenregistrer)
{
    int i;
    for(i = *nbScore; i > 0 && tab[i - 1].score < scoreAenregistrer.score; i--)
    {
        tab[i] = tab[i - 1];
    }
    if(*nbScore != 10)
    {
        *nbScore += 1;
    }
    if(i != 10)
    {
        printf("\n---------------------------------------------------------------------------\n");
        printf("\n\tBravo! Votre score va etre enregistre !\n");
        printf("\tEntrez votre nom : ");
        scanf("%s", scoreAenregistrer.nomJoueur);
        tab[i] = scoreAenregistrer;
    }
}
void enregistrer_topscore(int score, char *nomLaby)
{
    FILE* fichier;
    char nomFichier[31];
    strcpy(nomFichier, nomLaby);
    strcat(nomFichier, ".cfg");
    fichier = fopen(nomFichier, "a+");
    fseek(fichier, 0, SEEK_SET);
    int auMoinsUnJoueur = 0;
    int enregistrementNom = 1;
    scoreJoueur scoreAenregistrer;
    scoreJoueur scoreLu;
    scoreAenregistrer.score = score;
    scoreJoueur tableauScore[10];
    int nbScore = 0;
    char chaineTemporaire[31];
    char ligneAinserer[51];
    int position_caractere = 0;
    char caractereLu;

    memset(&scoreLu.nomJoueur[0], 0, sizeof(scoreLu.nomJoueur));
    memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));

    if(fichier != NULL)
    {
        while((caractereLu = fgetc(fichier)) != EOF)
        {
            if(caractereLu == '\n'){
                scoreLu.score = atoi(chaineTemporaire);
                tableauScore[nbScore] = scoreLu;
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

                    }
                    else
                    {
                        chaineTemporaire[position_caractere] = caractereLu;
                        position_caractere += 1;
                    }
                }
            }
            auMoinsUnJoueur = 1;
        }
        if(auMoinsUnJoueur == 1)
        {
            scoreLu.score = atoi(chaineTemporaire);
            tableauScore[nbScore] = scoreLu;
            memset(&scoreLu.nomJoueur[0], 0, sizeof(scoreLu.nomJoueur));
            memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));
            enregistrementNom = 1;
            position_caractere = 0;
            nbScore += 1;
        }

        if(nbScore == 0)
        {
            printf("\n---------------------------------------------------------------------------\n");
            printf("\n\tBravo! Votre score va etre enregistre !\n");
            printf("\tEntrez votre nom : ");
            scanf("%s", scoreAenregistrer.nomJoueur);
            //itoa(scoreAenregistrer.score, chaineTemporaire, 10);
            sprintf(chaineTemporaire, "%d", scoreAenregistrer.score);
            strcpy(ligneAinserer, scoreAenregistrer.nomJoueur);
            strcat(ligneAinserer, ":");
            strcat(ligneAinserer, chaineTemporaire);
            fputs(ligneAinserer, fichier);
            printf("\n--CLASSEMENT----------------------------------------------------------------\n");
            printf("\n             Nom                           Score                            \n\n");
            printf("\t%2d : %-30s%d\n", 1, scoreAenregistrer.nomJoueur, scoreAenregistrer.score);
        }
        else
        {
            int *p_nbScore = &nbScore;
            int i;
            verifier_position_score(&tableauScore[0], p_nbScore, scoreAenregistrer);
            fclose(fichier);
            fichier = fopen(nomFichier, "w+");
            printf("\n--CLASSEMENT----------------------------------------------------------------\n");
            printf("\n             Nom                           Score                            \n\n");
            for(i = 0; i < nbScore; i++)
            {
                memset(&ligneAinserer[0], 0, sizeof(ligneAinserer));
                memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));
                //itoa(tableauScore[i].score, chaineTemporaire, 10);
                sprintf(chaineTemporaire, "%d", tableauScore[i].score);
                strcpy(ligneAinserer, tableauScore[i].nomJoueur);
                strcat(ligneAinserer, ":");
                strcat(ligneAinserer, chaineTemporaire);
                printf("\t%2d : %-30s%d\n", i + 1, tableauScore[i].nomJoueur, tableauScore[i].score);
                if(i != 0)
                {
                    fputs("\n", fichier);
                }
                fputs(ligneAinserer, fichier);
            }
        }
        fclose(fichier);
    }
    else
    {
        printf("Impossible d'ouvrir\n");
    }
}
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe)
{
    clear();
    laby[nbColonnes * 1 + 0] = 'o';
    int score = 0;
    int scoreTotal = 0;
    int *p_score = &score;
    afficher_labyrinthe(laby, nbLignes, nbColonnes, score);
    char touche = 'l';
    fflush(stdin);
    time_t debut, fin;
    debut = time(NULL);
    while(touche != 'f')
    {
        touche = getch();
        switch(touche)
        {
            case 'd': deplacerAdroite(laby, nbLignes, nbColonnes, p_score);
                        break;
            case 'q': deplacerAgauche(laby, nbLignes, nbColonnes, p_score);
                        break;
            case 's': deplacerEnBas(laby, nbLignes, nbColonnes, p_score);
                        break;
            case 'z': deplacerEnHaut(laby, nbLignes, nbColonnes, p_score);
                        break;
        }
        if(verifier_position(nbLignes, nbColonnes)==1)
        {
            fin = time(NULL);
            int tempsPartie = (int)difftime(fin, debut);
            printf("\tMalus de temps : %d\n", tempsPartie / 2);
            scoreTotal = score - tempsPartie / 2;
            if(scoreTotal < 0)
            {
                scoreTotal = 0;
            }
            printf("\tScore total    : %d\n", scoreTotal);
            enregistrer_topscore(scoreTotal, nomLabyrinthe);
            touche = 'f';
        }
    }

    positionActuelle_X = 1;
    positionActuelle_Y = 0;
    laby[nbColonnes * (nbLignes-2) + (nbColonnes - 1)] = ' ';
    return;
}
