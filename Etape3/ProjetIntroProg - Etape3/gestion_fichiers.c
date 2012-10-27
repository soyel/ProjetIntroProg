#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestion_fichiers.h"

/**
 * Remplit un fichier avec un labyrinthe initialisé
 * @param fichier le fichier à remplir
 * @param laby le labyrinthe initlialisé
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 */
void remplir_fichier(FILE *fichier, char *laby, size_t nbLignes, size_t nbColonnes)
{
    char chaineTemporaire[4];
    sprintf(chaineTemporaire, "%d", (int) nbLignes);

    fputs(chaineTemporaire, fichier);
    fputs("\n", fichier);

    memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));
    sprintf(chaineTemporaire, "%d", (int) nbColonnes);

    fputs(chaineTemporaire, fichier);
    fputs("\n", fichier);

    size_t i;
    size_t j;

    for(i=0; i<nbLignes; i++)
    {
        for(j=0; j<nbColonnes; j++)
        {
            fputc(laby[nbColonnes * i + j], fichier);
        }
        fputs("\n", fichier);
    }
}

/**
 * Lit un labyrinthe à partir d'un fichier, et le stock dans laby
 * @param fichier fichier à lire
 * @param laby labyrinthe de stockage
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 */
void lire_fichier_labyrinthe(FILE *fichier, char *laby, size_t nbLignes, size_t nbColonnes)
{
    char caractereLu;
    size_t i, j;
    for(i=0; i<nbLignes; i++)
    {
        for(j=0; j<nbColonnes; j++)
        {
            do
            {
                 caractereLu = fgetc(fichier);
            }while(caractereLu == '\n');
            laby[nbColonnes * i + j] = caractereLu;
        }
    }
    return;
}
