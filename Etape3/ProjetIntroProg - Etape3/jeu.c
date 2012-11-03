#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef __WIN32__
    #include <conio.h>
    #define clear() system("cls")
#else
    #include "getch_tool.h"
    #define clear() system("clear")
#endif
#include "jeu.h"
#include "affichage.h"
#include "tools.h"

int positionActuelle_X = 1;
int positionActuelle_Y = 0;

/**
 * Vérifie que la diréction dans laquelle on veut se déplacer n'est pas un mur
 * @param x abscisse de la direction
 * @param y ordonnée de la direction
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
 * Vérifie que la direction dans la quelle on veut se déplacer
 * contient un trésor, ou un piège
 * @param laby le labyrinthe chargé
 * @param nbColones le nombre de colonnes du labyrinthe
 * @param bonusMalusX l'abscisse de la case ciblée
 * @param bonusMalusY l'ordonnée de la case ciblée
 * @return 1 si la case ciblée est un bonus, -1 si malus, 0 sinon
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
 * Vérifie si le joueur est positioné sur la case sortie
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @return 1 si le joueur est positioné sur la case sortie, 0 sinon
 */
int verifier_position_gagnante(size_t nbLignes, size_t nbColonnes)
{
    if(positionActuelle_X == (int)nbLignes - 2 && positionActuelle_Y == (int)nbColonnes - 1)
    {
        return 1;
    }
    return 0;
}

/**
 * Vérifie si le joueur a établi un score meilleur à ceux déjà enregistrés.
 * Dans ce cas là, il place son score dans un tableau de scores
 * @param tableauScores le tableau contenant les scores déjà enregistrés
 * @param nbScore le nombre de scores déjà enregistrés, soit la taille du tableau
 * @param scoreAenregistrer le score à enregistrer, avec le nom du joueur, ainsi que son score
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
		vider_buffer();
        tableauScores[i] = scoreAenregistrer;
    }
}

/**
 * Recherche si un monstre est situe sur le labyrinthe, aux coordonnees x et y
 * @param x l'abscisse de la case testee
 * @param y l'ordonnee de la case testee
 * @param tabMonstres le tableau des monstres
 * @param tailleTabMonstre la taille du tableau des monstres
 * @return n s'il n'y a aucun monstre aux coordonnees x:y, sinon renvoie le type de monstre
 */
char rechercher_monstre(int x, int y, Monstre *tabMonstres, int tailleTabMonstre)
{
    int i;
    for(i = 0; i < tailleTabMonstre; i++)
    {
        if(tabMonstres[i].positionX == x && tabMonstres[i].positionY == y)
        {
            return tabMonstres[i].typeMonstre;
        }
    }
    return 'n';
}

 /**
 * Déplace le joueur dans une direction indiquée, en fonction de
 * sa position actuelle, à condition que cette direction ne soit pas un mur
 * @param laby le labyrinthe charge
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @param directionX l'abscisse de la direction
 * @param directionY l'ordonnée de la direction
 * @param score le score du joueur
 */
void deplacer(char *laby, size_t nbLignes, size_t nbColonnes, int directionX, int directionY, int *score, char *nomLabyrinthe, Monstre *tabMonstres, int tailleTabM, int temps)
{
    if(verifier_possibilite(laby, nbColonnes, directionX, directionY) == 1)
    {
        *score += 10 * verifier_bonus_malus(laby, nbColonnes, directionX, directionY);
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = ' ';
        positionActuelle_X += directionX;
        positionActuelle_Y += directionY;
        laby[nbColonnes * positionActuelle_X + positionActuelle_Y] = 'o';
        clear();
        afficher_labyrinthe(laby, nbLignes, nbColonnes, *score, nomLabyrinthe, tabMonstres, tailleTabM, temps);
    }
    return;
}

/**
 * Génère un itinéraire permettant de sortir du labyrinthe laby,
 * en utilisant le principe dela main droite
 * @param laby le labyrinthe
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 */
void trouver_chemin_de_sortie(char *laby, size_t nbLignes, size_t nbColonnes)
{
    int position_Actuelle_X = 1;
    int position_Actuelle_Y = 0;

    int direction_x = 0;
    int direction_y = 1;

    int i;
    int j;

    clear();
    printf("\n--SOLUTION-----------------------------------------------------------------\n\n");
    while(position_Actuelle_X != (int)nbLignes - 2 || position_Actuelle_Y != (int)nbColonnes - 1)
    {
        /*déplacement à droite*/
        if(direction_x == 0 && direction_y == 1)
        {
            /*si là où on va n'est pas un mur*/
            if(laby[(int)nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] != '#')
            {
                /*si là où va est -, on y met +*/
                if(laby[(int)nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '.')
                {
                    laby[(int)nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] = '+';
                    if(laby[(int)nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        laby[(int)nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }

                /*si là où on va est +, on vérifie derriere*/
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '+')
                {
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                /*si là où on est n'est pas +, on y met -*/
                if(laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] != '+')
                {
                    laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] = '.';
                }


                /*si à droite de là où on va n'est pas un mur*/
                if(laby[nbColonnes * (position_Actuelle_X + 1) + (position_Actuelle_Y + 1)] != '#')
                {
                    direction_x = 1;
                    direction_y = 0;
                }

                position_Actuelle_X += 0;
                position_Actuelle_Y += 1;
            }
            /*si c'est un mur*/
            else
            {
                direction_x = -1;
                direction_y = 0;
            }
        }
        /*direction à gauche*/
        else if(direction_x == 0 && direction_y == -1)
        {
            if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] != '#')
            {
                /*si là où va est -, on y met +*/
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '.')
                {
                    laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] = '+';
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }

                /*si là où on va est +, on vérifie derriere*/
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '+')
                {
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                /*si là où on est n'est pas +, on y met -*/
                if(laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] != '+')
                {
                    laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] = '.';
                }

                /*si à droite de là où on va n'est pas un mur*/
                if(laby[nbColonnes * (position_Actuelle_X - 1) + (position_Actuelle_Y - 1)] != '#')
                {
                    direction_x = -1;
                    direction_y = 0;
                }

                position_Actuelle_X += 0;
                position_Actuelle_Y += -1;
            }
            /*si c'est un mur*/
            else
            {
                direction_x = 1;
                direction_y = 0;
            }
        }
        /*direction en haut*/
        else if(direction_x == -1 && direction_y == 0)
        {
            /*si là où on va n'est pas un mur*/
            if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] != '#')
            {
                /*si là où va est -, on y met +*/
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '.')
                {
                    laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] = '+';
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                /*si là où on va est +, on vérifie derriere*/
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '+')
                {
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }

                /*si là où on est n'est pas +, on y met -*/
                if(laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] != '+')
                {
                    laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] = '.';
                }

                /*si à droite de là où on va n'est pas un mur*/
                if(laby[nbColonnes * (position_Actuelle_X - 1) + (position_Actuelle_Y + 1)] != '#')
                {
                    direction_x = 0;
                    direction_y = 1;
                }

                position_Actuelle_X += -1;
                position_Actuelle_Y += 0;

            }
            /*si c'est un mur*/
            else
            {
                direction_x = 0;
                direction_y = -1;
            }
        }
        /*direction en bas*/
        else
        {
            if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] != '#')
            {
                /*si là où va est -, on y met +*/
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '.')
                {
                    laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] = '+';
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                /*si là où on va est +, on vérifie derriere*/
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '+')
                {
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                /*si là où on est n'est pas +, on y met -*/
                if(laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] != '+')
                {
                    laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] = '.';
                }

                /*si à droite de là où on va n'est pas un mur*/
                if(laby[nbColonnes * (position_Actuelle_X + 1) + (position_Actuelle_Y - 1)] != '#')
                {
                    direction_x = 0;
                    direction_y = -1;
                }

                position_Actuelle_X += 1;
                position_Actuelle_Y += 0;
            }
            else
            {
                direction_x = 0;
                direction_y = 1;
            }
        }
    }

    laby[nbColonnes * 1 + 0] = '.';
    laby[nbColonnes * (nbLignes - 2) + (nbColonnes - 1)] = '.';

    for(i = 0; i < (int)nbLignes; i++)
    {
        printf("\t");
        for(j = 0; j < (int)nbColonnes; j++)
        {
            if(laby[nbColonnes * i + j] == '+' || laby[nbColonnes * i + j] == 'm' || laby[nbColonnes * i + j] == 'b' || laby[nbColonnes * i + j] == 'f' || laby[nbColonnes * i + j] == 'g')
            {
                laby[nbColonnes * i + j] = ' ';
            }
            if(laby[nbColonnes * (i - 1) + j] == '.' && laby[nbColonnes * (i + 1) + j] == '.' && laby[nbColonnes * i + j] != '#')
            {
                laby[nbColonnes * i + j] = '.';
            }
            if(laby[nbColonnes * i + (j - 1)] == '.' && laby[nbColonnes * i + (j + 1)] == '.' && laby[nbColonnes * i + j] != '#')
            {
                laby[nbColonnes * i + j] = '.';
            }
            if(laby[nbColonnes * (i - 1) + j] == '.' && laby[nbColonnes * i + (j + 1)] == '.' && laby[nbColonnes * i + j] != '#')
            {
                laby[nbColonnes * i + j] = '.';
            }
            if(laby[nbColonnes * i + (j - 1)] == '.' && laby[nbColonnes * (i - 1) + j] == '.' && laby[nbColonnes * i + j] != '#')
            {
                laby[nbColonnes * i + j] = '.';
            }
            if(laby[nbColonnes * i + (j + 1)] == '.' && laby[nbColonnes * (i + 1) + j] == '.' && laby[nbColonnes * i + j] != '#')
            {
                laby[nbColonnes * i + j] = '.';
            }
            if(laby[nbColonnes * i + (j - 1)] == '.' && laby[nbColonnes * (i + 1) + j] == '.' && laby[nbColonnes * i + j] != '#')
            {
                laby[nbColonnes * i + j] = '.';
            }
            printf("%c", laby[nbColonnes * i + j]);
        }
        printf("\n");
    }

}

/**
 * Enregistre le score effectué par le joueur dans le cas où il serait supérieur
 * aux dix meilleurs scores déjà enregistrés
 * @param score le score effectué par le joueur
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

    scoreJoueur scoreAenregistrer;
    scoreJoueur scoreLu;
    scoreJoueur tableauScore[10];

    FILE* fichier;

    strcpy(nomFichier, nomLaby);
    strcat(nomFichier, ".cfg");

    fichier = fopen(nomFichier, "a+");
    fseek(fichier, 0, SEEK_SET);


    scoreAenregistrer.score = score;
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
			vider_buffer();
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
 * Analyse la situation initiale du labyrinthe, avant que le joueur ne fasse son premier pas,
 * et stock les coordonnees et le type des monstres situes sur le labyrinthe
 * @param laby le labyrinthe de la partie
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @param tailleTabMonstres la taille du tableau de monstres
 * @return le tableau des monstres
 */
Monstre *analyser_monstres(char *laby, size_t nbLignes, size_t nbColonnes, int *tailleTabMonstres)
{
    size_t i;
    size_t j;

    Monstre *tabMonstres;

    int compteurNombreMonstres;
    compteurNombreMonstres = 0;

    for(i = 0; i < nbLignes; i++)
    {
        for(j = 0; j < nbColonnes; j++)
        {
            if(laby[nbColonnes * i + j] == 'g' || laby[nbColonnes * i + j] == 'f')
            {
                compteurNombreMonstres ++;
            }
        }
    }

    tabMonstres = malloc (compteurNombreMonstres * sizeof(Monstre));
    compteurNombreMonstres = 0;
    for(i = 0; i < nbLignes; i++)
    {
        for(j = 0; j < nbColonnes; j++)
        {
            if(laby[nbColonnes * i + j] == 'g' || laby[nbColonnes * i + j] == 'f')
            {
                tabMonstres[compteurNombreMonstres].typeMonstre = laby[nbColonnes * i + j];
                tabMonstres[compteurNombreMonstres].positionX = i;
                tabMonstres[compteurNombreMonstres].positionY = j;
                tabMonstres[compteurNombreMonstres].penalite = (rand()%2) + 1 ;
                compteurNombreMonstres ++;
                laby[nbColonnes * i + j] = ' ';
            }
        }
    }

    *tailleTabMonstres = compteurNombreMonstres;
    return tabMonstres;
}

/**
 * Deplace les monstres sur le labyrinthe,
 * en fonction de leur type
 * @param tabMonstres le tableau des monstres
 * @param nbL le nombre de lignes du labyrinthe
 * @param nbC le nombre de colonnes du labyrinthe
 * @param tailleTabM la taille du tableau de monstres
 * @param le labyrinthe
 */
void deplacer_monstres(Monstre *tabMonstres, size_t nbL, size_t nbC, int tailleTabM, char *laby)
{

    int i;
    int continuerDirection;
    int directionAprendre;

    for(i = 0; i < tailleTabM; i++)
    {
        if(tabMonstres[i].typeMonstre == 'f')
        {
            continuerDirection = 1;
            directionAprendre = rand()%4 + 1;
            while(continuerDirection)
            {
                if(directionAprendre == 1)
                {
                    if(tabMonstres[i].positionX - tabMonstres[i].penalite > 0)
                    {
                        tabMonstres[i].positionX -= tabMonstres[i].penalite;
                        continuerDirection = 0;
                    }
                    else
                    {
                        if(tabMonstres[i].penalite > 1 && tabMonstres[i].positionX - (tabMonstres[i].penalite - 1) > 0)
                        {
                            tabMonstres[i].positionX = tabMonstres[i].positionX - (tabMonstres[i].penalite - 1);
                            continuerDirection = 0;
                        }
                        else
                        {
                            directionAprendre +=1;
                        }
                    }
                }
                if(directionAprendre == 2)
                {
                    if(tabMonstres[i].positionY + tabMonstres[i].penalite < (int)nbC - 1)
                    {
                        tabMonstres[i].positionY += tabMonstres[i].penalite;
                        continuerDirection = 0;
                    }
                    else
                    {
                        if(tabMonstres[i].penalite > 1 && tabMonstres[i].positionY + (tabMonstres[i].penalite - 1) < (int)nbC - 1)
                        {
                            tabMonstres[i].positionY = tabMonstres[i].positionY + (tabMonstres[i].penalite - 1);
                            continuerDirection = 0;
                        }
                        else
                        {
                            directionAprendre +=1;
                        }
                    }
                }
                if(directionAprendre == 3)
                {
                    if(tabMonstres[i].positionX + tabMonstres[i].penalite < (int)nbL - 1)
                    {
                        tabMonstres[i].positionX += tabMonstres[i].penalite;
                        continuerDirection = 0;
                    }
                    else
                    {
                        if(tabMonstres[i].penalite > 1 && tabMonstres[i].positionX + (tabMonstres[i].penalite - 1) < (int)nbL - 1)
                        {
                            tabMonstres[i].positionX = tabMonstres[i].positionX + (tabMonstres[i].penalite - 1);
                            continuerDirection = 0;
                        }
                        else
                        {
                            directionAprendre +=1;
                        }
                    }
                }
                if(directionAprendre == 4)
                {
                    if(tabMonstres[i].positionY - tabMonstres[i].penalite > 0)
                    {
                        tabMonstres[i].positionY -= tabMonstres[i].penalite;
                        continuerDirection = 0;
                    }
                    else
                    {
                        if(tabMonstres[i].penalite > 1 && tabMonstres[i].positionY - (tabMonstres[i].penalite - 1) > 0)
                        {
                            tabMonstres[i].positionY = tabMonstres[i].positionY - (tabMonstres[i].penalite - 1);
                            continuerDirection = 0;
                        }
                        else
                        {
                            directionAprendre = 1;
                        }
                    }
                }
            }
        }
        else
        {
            continuerDirection = tabMonstres[i].penalite;
            directionAprendre = rand()%4 + 1;
            while(continuerDirection != 0)
            {
                if(directionAprendre == 1)
                {
                    if(tabMonstres[i].positionX - 1 > 0 && laby[nbC * (tabMonstres[i].positionX - 1) + tabMonstres[i].positionY] != '#')
                    {
                        tabMonstres[i].positionX -= 1;
                        continuerDirection -= 1;
                    }
                    else
                    {
                        directionAprendre += 1;
                    }
                }
                if(directionAprendre == 2)
                {
                    if(tabMonstres[i].positionY + 1 < (int)nbC - 1  && laby[nbC * tabMonstres[i].positionX + (tabMonstres[i].positionY + 1)] != '#')
                    {
                        tabMonstres[i].positionY += 1;
                        continuerDirection -= 1;
                    }
                    else
                    {
                        directionAprendre += 1;
                    }
                }
                if(directionAprendre == 3)
                {
                    if(tabMonstres[i].positionX + 1 < (int)nbL - 1 && laby[nbC * (tabMonstres[i].positionX + 1) + tabMonstres[i].positionY] != '#')
                    {
                        tabMonstres[i].positionX += 1;
                        continuerDirection -= 1;
                    }
                    else
                    {
                        directionAprendre += 1;
                    }
                }
                if(directionAprendre == 4)
                {
                    if(tabMonstres[i].positionY - 1 > 0  && laby[(int)nbC * tabMonstres[i].positionX + (tabMonstres[i].positionY - 1)] != '#')
                    {
                        tabMonstres[i].positionY -= 1;
                        continuerDirection -= 1;
                    }
                    else
                    {
                        directionAprendre = 1;
                    }
                }
            }
        }
    }
}
/**
 * Joue une partie jusqu'à ce que le joueur n'atteigne la sortie,
 * ou appuie sur la touche 'f'
 * @param laby le labyrinthe chargé
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @param nomLabyrinthe le nom du labyrinthe
 * @param avecClassement partie avec classement ou non
 */
void jouer(char *laby, size_t nbLignes, size_t nbColonnes, char *nomLabyrinthe)
{
    int score = 0;
    int scoreTotal = 0;
    int *p_score = &score;

    char touche = 'l';

    time_t debut = time(NULL);
    time_t fin;
    int tempsPartie;
    int tailleTabMonstres;
    Monstre *tabMonstres = analyser_monstres(laby, nbLignes, nbColonnes, &tailleTabMonstres);
    clear();
    laby[nbColonnes * 1 + 0] = 'o';
    afficher_labyrinthe(laby, nbLignes, nbColonnes, score, nomLabyrinthe, tabMonstres, tailleTabMonstres, 0);

    vider_buffer();
    while(touche != 'f')
    {
        touche = getch();
        switch(touche)
        {
            case 'd':   fin = time(NULL);
                        deplacer_monstres(tabMonstres, nbLignes, nbColonnes, tailleTabMonstres, laby);
                        deplacer(laby, nbLignes, nbColonnes, 0, 1, p_score, nomLabyrinthe, tabMonstres, tailleTabMonstres, (int)difftime(fin, debut));
                        break;

            case 'q':   fin = time(NULL);
                        deplacer_monstres(tabMonstres, nbLignes, nbColonnes, tailleTabMonstres, laby);
                        deplacer(laby, nbLignes, nbColonnes, 0, -1, p_score, nomLabyrinthe, tabMonstres, tailleTabMonstres, (int)difftime(fin, debut));
                        break;

            case 's':   fin = time(NULL);
                        deplacer_monstres(tabMonstres, nbLignes, nbColonnes, tailleTabMonstres, laby);
                        deplacer(laby, nbLignes, nbColonnes, 1, 0, p_score, nomLabyrinthe, tabMonstres, tailleTabMonstres, (int)difftime(fin, debut));
                        break;

            case 'z':   fin = time(NULL);
                        deplacer_monstres(tabMonstres, nbLignes, nbColonnes, tailleTabMonstres, laby);
                        deplacer(laby, nbLignes, nbColonnes, -1, 0, p_score, nomLabyrinthe, tabMonstres, tailleTabMonstres, (int)difftime(fin, debut));
                        break;
        }
        if(verifier_position_gagnante(nbLignes, nbColonnes)==1)
        {
            fin = time(NULL);
            tempsPartie = (int)difftime(fin, debut);
            scoreTotal = score - tempsPartie / 2;

            printf("\tMalus de temps : -%d points\n", tempsPartie / 2);

            if(scoreTotal < 0)
            {
                scoreTotal = 0;
            }
            printf("\tScore total    : %d points\n", scoreTotal);
            enregistrer_topscore(scoreTotal, nomLabyrinthe);
            touche = 'f';
        }
    }

    positionActuelle_X = 1;
    positionActuelle_Y = 0;
    laby[nbColonnes * (nbLignes-2) + (nbColonnes - 1)] = ' ';

    return;
}
