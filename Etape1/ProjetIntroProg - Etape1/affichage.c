#incldue <stdlib.h>
#include <stdio.h>

#include "affichage.h"

/**
 * Affiche le labyrinthe laby
 */
void afficher_labyrinthe()
{
    int i, j;
    for(i=0; i<LIGNES; i++)
    {
        for(j=0; j<COLONNES; j++)
        {
            if(laby[i][j]=='H' || laby[i][j]=='V')
            {
                laby[i][j]='#';
            }
            printf("%c", laby[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Aide: Faites deplacer le symbole 'o' vers la sortie, situee en bas a droite.\n");
    return;
}

/**
 * Affiche le menu d'accueil, qui permet de démarrer une partie ou quitter le jeu
 * @return 0 si le joueur décide de quitter le jeu, 1 sinon
 */
int afficher_accueil()
{
    printf("################################################\n");
    printf("################################################\n");
    printf("###                                          ###\n");
    printf("###                                          ###\n");
    printf("###    ##          #      #####     ##  ##   ###\n");
    printf("###    ##          #      ##  ##     ####    ###\n");
    printf("###    ##         # #     ######      ##     ###\n");
    printf("###    ##        #####    ##  ##      ##     ###\n");
    printf("###    ######   ##   ##   #####       ##     ###\n");
    printf("###                                          ###\n");
    printf("###                1. Jouer                  ###\n");
    printf("###                2. Quitter                ###\n");
    printf("###                                          ###\n");
    printf("###                                          ###\n");
    printf("###  Copyrights R. Kos - ENSICAEN 2012-2013  ###\n");
    printf("###                                          ###\n");
    printf("################################################\n");
    printf("################################################\n");

    int touche = 'o';
    while(touche!='2' && touche!='1')
    {
        touche = getch();
    }
    if(touche == '2')
    {
        return 0;
    }
    return 1;

}
