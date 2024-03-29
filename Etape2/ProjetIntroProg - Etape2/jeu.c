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

/**
 * V�rifie que la dir�ction dans laquelle on veut se d�placer n'est pas un mur
 * @param x abscisse de la direction
 * @param y ordonn�e de la direction
 * @return 0 si la direction est un mur, 0 sinon
 */
int verifier_possibilite(char *laby, size_t nbColonnes, int x, int y)
{
    if(laby[nbColonnes * (positionActuelle_X + x) + (positionActuelle_Y + y)]!='#')
    {
        return 1;
    }
    return 0;
}

/**
 * V�rifie que la direction dans la quelle on veut se d�placer
 * contient un tr�sor, ou un pi�ge
 * @param laby le labyrinthe charg�
 * @param nbColones le nombre de colonnes du labyrinthe
 * @param bonusMalusX l'abscisse de la case cibl�e
 * @param bonusMalusY l'ordonn�e de la case cibl�e
 * @return 1 si la case cibl�e est un bonus, -1 si malus, 0 sinon
 */
int verifier_bonus_malus(char *laby, size_t nbColonnes, int bonusMalusX, int bonusMalusY)
{
    if(laby[nbColonnes * (positionActuelle_X + bonusMalusX) + (positionActuelle_Y + bonusMalusY)] == 'b')
    {
        return 1;
    }
    else if(laby[nbColonnes * (positionActuelle_X + bonusMalusX) + (positionActuelle_Y + bonusMalusY)] == 'm')
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
 * V�rifie si le joueur est position� sur la case sortie
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @return 1 si le joueur est position� sur la case sortie, 0 sinon
 */
int verifier_position_gagnante(size_t nbLignes, size_t nbColonnes)
{
    if(positionActuelle_X == nbLignes - 2 && positionActuelle_Y == nbColonnes - 1)
    {
        return 1;
    }
    return 0;
}

/**
 * V�rifie si le joueur a �tabli un score meilleur � ceux d�j� enregistr�s.
 * Dans ce cas l�, il place son score dans un tableau de scores
 * @param tableauScores le tableau contenant les scores d�j� enregistr�s
 * @param nbScore le nombre de scores d�j� enregistr�s, soit la taille du tableau
 * @param scoreAenregistrer le score � enregistrer, avec le nom du joueur, ainsi que son score
 */
void verifier_position_score(scoreJoueur* tableauScores, int *nbScore, scoreJoueur scoreAenregistrer)
{
    int i;
    for(i = *nbScore; i > 0 && tableauScores[i - 1].score < scoreAenregistrer.score; i--)
    {
        tableauScores[i] = tableauScores[i - 1];
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
        tableauScores[i] = scoreAenregistrer;
    }
}

 /**
 * D�place le joueur dans une direction indiqu�e, en fonction de
 * sa position actuelle, � condition que cette direction ne soit pas un mur
 * @param laby le labyrinthe charge
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @param directionX l'abscisse de la direction
 * @param directionY l'ordonn�e de la direction
 * @param score le score du joueur
 */
void deplacer(char *laby, size_t nbLignes, size_t nbColonnes, int directionX, int directionY, int *score)
{
    //if(verifier_possibilite(laby, nbColonnes, 1, 0) == 1)
    if(verifier_possibilite(laby, nbColonnes, directionX, directionY) == 1)
    {
        *score += 10 * verifier_bonus_malus(laby, nbColonnes, directionX, directionY);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_X += directionX;
        positionActuelle_Y += directionY;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        clear();
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score);
    }
    return;
}

/**
 * Enregistre le score effectu� par le joueur dans le cas o� il serait sup�rieur
 * aux dix meilleurs scores d�j� enregistr�s
 * @param score le score effectu� par le joueur
 * @param nomLaby le nom du labyrinthe
 */
void enregistrer_topscore(int score, char *nomLaby)
{
    char nomFichier[31];
    char chaineTemporaire[31];
    char ligneAinserer[51];
    char caractereLu;

    int auMoinsUnJoueur = 0;
    int enregistrementNom = 1;
    int nbScore = 0;
    int position_caractere = 0;

    strcpy(nomFichier, nomLaby);
    strcat(nomFichier, ".cfg");

    FILE* fichier;
    fichier = fopen(nomFichier, "a+");
    fseek(fichier, 0, SEEK_SET);

    scoreJoueur scoreAenregistrer;
    scoreAenregistrer.score = score;
    scoreJoueur scoreLu;
    scoreJoueur tableauScore[10];

    memset(&scoreLu.nomJoueur[0], 0, sizeof(scoreLu.nomJoueur));
    memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));

    if(fichier != NULL)
    {
        while((caractereLu = fgetc(fichier)) != EOF)
        {
            if(caractereLu == '\n')
            {
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

/**
 * Joue une partie jusqu'� ce que le joueur n'atteigne la sortie,
 * ou appuie sur la touche 'f'
 * @param laby le labyrinthe charg�
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @param nomLabyrinthe le nom du labyrinthe
 * @param avecClassement partie avec classement ou non
 */
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe, int avecClassement)
{
    clear();
    laby[nbColonnes * 1 + 0] = 'o';

    int score = 0;
    int scoreTotal = 0;
    int *p_score = &score;

    char touche = 'l';

    time_t debut = time(NULL);
    time_t fin;

    afficher_labyrinthe(laby, nbLignes, nbColonnes, score);

    fflush(stdin);
    while(touche != 'f')
    {
        touche = getch();
        switch(touche)
        {
            case 'd': deplacer(laby, nbLignes, nbColonnes, 0, 1, p_score);
                        break;
            case 'q': deplacer(laby, nbLignes, nbColonnes, 0, -1, p_score);
                        break;
            case 's': deplacer(laby, nbLignes, nbColonnes, 1, 0, p_score);
                        break;
            case 'z': deplacer(laby, nbLignes, nbColonnes, -1, 0, p_score);
                        break;
        }
        if(verifier_position_gagnante(nbLignes, nbColonnes)==1)
        {
            fin = time(NULL);
            int tempsPartie = (int)difftime(fin, debut);
            scoreTotal = score - tempsPartie / 2;

            printf("\tMalus de temps : %d\n", tempsPartie / 2);

            if(scoreTotal < 0)
            {
                scoreTotal = 0;
            }
            printf("\tScore total    : %d\n", scoreTotal);
            if(avecClassement == 1)
            {
                enregistrer_topscore(scoreTotal, nomLabyrinthe);
            }
            else
            {
                printf("\n---------------------------------------------------------------------------\n");
            }
            touche = 'f';
        }
    }

    positionActuelle_X = 1;
    positionActuelle_Y = 0;
    laby[nbColonnes * (nbLignes-2) + (nbColonnes - 1)] = ' ';

    return;
}
