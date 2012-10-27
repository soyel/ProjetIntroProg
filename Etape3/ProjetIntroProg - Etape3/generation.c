#include <stdio.h>
#include <stdlib.h>

#include "generation.h"

/**
 * Initialise chaque cellule comme un début de chemin
 * @param id identifiant de la cellule
 * @param x abscisse de la cellule
 * @param y ordonnée de la cellule
 * @return la cellule comme un chemin
 */
chemin initialiserChemin(int id, int x, int y)
{
    cellule* nouvelleCellule = malloc(sizeof(cellule));

    nouvelleCellule->identifiant = id;
    nouvelleCellule->x = x;
    nouvelleCellule->y = y;

    nouvelleCellule->celluleSuivante = NULL;
    nouvelleCellule->premiereCellule = nouvelleCellule;

    return nouvelleCellule;
}

/**
 * Ajoute leChemin, qui peut être une cellule ou un chemin de cellule,
 * à la fin du chemin lesChemins
 * @param lesChemins le chemin principal principal d'une ou plusieurs cellules
 * @param leChemin le chemin à ajouter
 */
void ajouterEnfinDeListe(chemin lesChemins, cellule* leChemin)
{
    cellule* temp = lesChemins;
    while(temp->celluleSuivante != NULL)
    {
        temp = temp->celluleSuivante;
    }
    temp->celluleSuivante = leChemin;
    return;
}

/**
 * Genère un labyrinthe de nbLignes lignes et nbColonnes colonnes
 * @param laby le labyrinthe vide
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 */
void creer_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes)
{
    int compteurCell = 1;
    int compteurMur = 1;
    int nbMurs = ((nbLignes * nbColonnes) - (nbColonnes * 2) - ((nbLignes-2)*2))/2;

    size_t i;
    size_t j;

    chemin tabDesChemins[nbLignes][nbColonnes];
    struct coordonneesMurs mesMurs[nbMurs];

    for(i = 0; i<nbLignes; ++i)
    {
        for(j = 0; j<nbColonnes; ++j)
        {
            if(i==0 ||j==0 || i == nbLignes - 1 || j == nbColonnes - 1)
            {
                laby[nbColonnes * i+j] = '#';
            }
            else
            {
                if(i%2!=0 && j%2!=0)
                {
                    laby[nbColonnes * i + j] = ' ';
                    tabDesChemins[i][j] = initialiserChemin(compteurCell, i, j);
                    compteurCell +=1;
                }
                else
                {
                    if(i%2 == 0 && j%2 == 0)
                    {
                        laby[nbColonnes * i + j] = '#';
                    }
                    else
                    {
                        mesMurs[compteurMur-1].x = i;
                        mesMurs[compteurMur-1].y = j;
                        if(i%2==0)
                        {
                            mesMurs[compteurMur-1].type = 'H';
                            laby[nbColonnes * i + j] = '#';
                        }
                        else
                        {
                            mesMurs[compteurMur-1].type = 'V';
                            laby[nbColonnes * i + j] = '#';
                        }
                        compteurMur +=1;
                    }
                }
            }
        }
    }

    compteurMur = nbMurs;
    int compteurAleatoire;
    int coordXTmp, coordYTmp;
    int coordXPremCell, coordYPremCell;
    int coordXSecCell, coordYSecCell;
    int idPremCell, idSecCell;
    int cellDominante, cell1, cell2;

    cellule* cellule1 = NULL;
    cellule* cellule2 = NULL;
    cellule* tmpDebutChemin = NULL;
    cellule* tmpCelluleSuivante = NULL;
    while(compteurMur > 0)
    {
        compteurAleatoire = rand()%compteurMur;
        coordXTmp = mesMurs[compteurAleatoire].x;
        coordYTmp = mesMurs[compteurAleatoire].y;
        cellDominante = rand()%2;
        if(cellDominante == 0)
        {
            cell1 = -1;
            cell2 = 1;
        }
        else
        {
            cell1 = 1;
            cell2 = -1;
        }
        if(mesMurs[compteurAleatoire].type=='H')
        {
            coordXPremCell = coordXTmp + cell1;
            coordXSecCell = coordXTmp + cell2;
            coordYPremCell = coordYTmp;
            coordYSecCell = coordYTmp;
        }
        else
        {
            coordXPremCell = coordXTmp;
            coordXSecCell = coordXTmp;
            coordYPremCell = coordYTmp + cell1;
            coordYSecCell = coordYTmp + cell2;
        }

        cellule1 = tabDesChemins[coordXPremCell][coordYPremCell];
        cellule2 = tabDesChemins[coordXSecCell][coordYSecCell];
        tmpDebutChemin = cellule2 ->premiereCellule;
        idPremCell = cellule1->identifiant;
        idSecCell = cellule2->identifiant;


        if(idPremCell != idSecCell)
        {
            laby[nbColonnes * coordXTmp + coordYTmp] = ' ';
            if((tmpDebutChemin->celluleSuivante)==NULL)
            {
                ajouterEnfinDeListe(cellule1->premiereCellule, cellule2);
                cellule2->premiereCellule = cellule1->premiereCellule;
                cellule2->identifiant = cellule1->identifiant;
            }
            else
            {
                tmpCelluleSuivante = tmpDebutChemin;
                while(tmpCelluleSuivante->celluleSuivante != NULL)
                {
                    tmpCelluleSuivante->identifiant = cellule1->identifiant;
                    tmpCelluleSuivante->premiereCellule = cellule1->premiereCellule;
                    tmpCelluleSuivante = tmpCelluleSuivante->celluleSuivante;
                }
                tmpCelluleSuivante->identifiant = cellule1->identifiant;
                tmpCelluleSuivante->premiereCellule = cellule1->premiereCellule;
                ajouterEnfinDeListe(cellule1->premiereCellule, tmpDebutChemin);
            }
        }
        mesMurs[compteurAleatoire].x = mesMurs[compteurMur-1].x;
        mesMurs[compteurAleatoire].y = mesMurs[compteurMur-1].y;
        mesMurs[compteurAleatoire].type = mesMurs[compteurMur-1].type;
        compteurMur -= 1;
    }

    /*int nbMalusAdistribuer = (((nbLignes * nbColonnes) - (nbColonnes * 2) - ((nbLignes-2)*2))/3)/2;
    int nbBonusAdistribuer = (((nbLignes * nbColonnes) - (nbColonnes * 2) - ((nbLignes-2)*2))/3)/2;*/

    int compteurCelluleVide = 0;
    int celluleAleatoire;
    int typeBonusMalus;
    char bonusMalus = 'o';

    struct coordonnees toutesCellulesVides[(nbLignes - 2)*(nbColonnes - 2)];
    for(i = 0; i < nbLignes; i++)
    {
        for(j = 0; j < nbColonnes; j++)
        {
            if(laby[nbColonnes * i + j] == ' ')
            {
                toutesCellulesVides[compteurCelluleVide].x = i;
                toutesCellulesVides[compteurCelluleVide].y = j;
                compteurCelluleVide += 1;
            }

        }
    }

    while(compteurCelluleVide > 0)
    {
        celluleAleatoire = rand()%compteurCelluleVide;
        typeBonusMalus = rand()%2;
        if(typeBonusMalus == 1)
        {
            bonusMalus = 'm';
        }
        else
        {
            bonusMalus = 'b';
        }
        typeBonusMalus = rand()%20;
        if(typeBonusMalus >=0 && typeBonusMalus <= 3)
        {
            laby[nbColonnes * toutesCellulesVides[celluleAleatoire].x + toutesCellulesVides[celluleAleatoire].y] = bonusMalus;
        }


        toutesCellulesVides[celluleAleatoire].x  = toutesCellulesVides[compteurCelluleVide-1].x;
        toutesCellulesVides[celluleAleatoire].y  = toutesCellulesVides[compteurCelluleVide-1].y;
        compteurCelluleVide -= 1;

    }
    laby[nbColonnes * 1 + 0] = ' ';
    laby[nbColonnes * (nbLignes-2) + (nbColonnes-1)] = ' ';

    return;
}
