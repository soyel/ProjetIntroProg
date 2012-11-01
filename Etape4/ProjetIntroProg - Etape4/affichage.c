#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <SDL.h>

#include "jeu.h"
#include "affichage.h"

#include "generation.h"
#include "gestion_fichiers.h"

/**
 * Affiche le labyrinthe laby
 * @param laby le labyrinthe a afficher
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @param score le score a afficher
 */
void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int score, char *nomLaby, Monstre *tabMonstres, int tailleTabMonstres, SDL_Surface *ecran, InterfaceCreationLaby iCreation)
{
    SDL_Surface *mur = NULL;
    SDL_Surface *solution = NULL;
    SDL_Surface *tresor = NULL;
    SDL_Surface *joueur = NULL;
    SDL_Surface *piege = NULL;
    SDL_Surface *fantome = NULL;
    SDL_Surface *leprechaun = NULL;

    SDL_Rect positionCase;

    positionCase.x = 50;
    positionCase.y = 50;

    char typeMonstre;

    mur = IMG_Load("images/mur.png");
    joueur = IMG_Load("images/kenny.png");
    solution = IMG_Load("images/solutionIcone.png");
    tresor = IMG_Load("images/tresor.png");
    piege = IMG_Load("images/piege.png");
    fantome = IMG_Load("images/fantome.png");
    leprechaun = IMG_Load("images/leprechaun.png");

    size_t i, j;

    SDL_BlitSurface(iCreation.imageDeFond2, NULL, ecran, &iCreation.positionImageDeFond2);
    SDL_BlitSurface(iCreation.retourMenu, NULL, ecran, &iCreation.positionRetourMenu);

    for(i = 0; i < nbLignes; i++)
    {
        for(j = 0; j < nbColonnes; j++)
        {
            if(laby[nbColonnes * i + j] == 'o')
            {
                SDL_BlitSurface(joueur, NULL, ecran, &positionCase);
            }
            if(laby[nbColonnes * i + j] == '#')
            {
                SDL_BlitSurface(mur, NULL, ecran, &positionCase);
            }
            if(laby[nbColonnes * i + j] == 'b')
            {
                SDL_BlitSurface(tresor, NULL, ecran, &positionCase);
            }
            if(laby[nbColonnes * i + j] == 'm')
            {
                SDL_BlitSurface(piege, NULL, ecran, &positionCase);
            }
            typeMonstre = rechercher_monstre(i, j, tabMonstres, tailleTabMonstres);
            if(typeMonstre != 'n' && laby[nbColonnes * i + j] != 'o')
            {
                if(typeMonstre == 'f')
                {
                    SDL_BlitSurface(fantome, NULL, ecran, &positionCase);
                }
                else
                {
                    SDL_BlitSurface(leprechaun, NULL, ecran, &positionCase);
                }

            }
            positionCase.x += 30;
        }
        positionCase.x = 50;
        positionCase.y += 30;
    }

    SDL_Flip(ecran);
    return;
}
