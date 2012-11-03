#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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
void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int scoreAct, char *nomLaby, Monstre *tabMonstres, int tailleTabMonstres, SDL_Surface *ecran, InterfaceCreationLaby iCreation, int tempsAct)
{
    SDL_Surface *mur = NULL;
    SDL_Surface *tresor = NULL;
    SDL_Surface *joueur = NULL;
    SDL_Surface *piege = NULL;
    SDL_Surface *fantome = NULL;
    SDL_Surface *leprechaun = NULL;
    SDL_Surface *scoreBar = NULL;
    SDL_Surface *score = NULL;
    SDL_Surface *temps = NULL;

    SDL_Rect positionCase;
    SDL_Rect positionScoreBar;
    SDL_Rect positionScore;
    SDL_Rect positionTemps;

    positionCase.x = 5;
    positionCase.y = 50;

    positionScoreBar.x = 0;
    positionScoreBar.y = 0;

    positionScore.x = 1110;
    positionScore.y = 6;

    positionTemps.x = 1035;
    positionTemps.y = 601;

    char typeMonstre;

    mur = IMG_Load("images/mur.png");
    joueur = IMG_Load("images/kenny.png");
    tresor = IMG_Load("images/tresor.png");
    piege = IMG_Load("images/piege.png");
    fantome = IMG_Load("images/fantome.png");
    leprechaun = IMG_Load("images/leprechaun.png");
    scoreBar = IMG_Load("images/scoreBar.png");

    TTF_Init();
    SDL_Color couleurBlanche = {255, 0, 0};

    TTF_Font *police = NULL;

    police = TTF_OpenFont("times.ttf", 24);

    char chaineTemporaire[8];
    sprintf(chaineTemporaire, "%d pts", scoreAct);

    score = TTF_RenderText_Blended(police, chaineTemporaire, couleurBlanche);

    memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));
    sprintf(chaineTemporaire, "%d sec", tempsAct);
    temps = TTF_RenderText_Blended(police, chaineTemporaire, couleurBlanche);
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
        positionCase.x = 5;
        positionCase.y += 30;
    }
    SDL_BlitSurface(scoreBar, NULL, ecran, &positionScoreBar);
    SDL_BlitSurface(score, NULL, ecran, &positionScore);
    SDL_BlitSurface(temps, NULL, ecran, &positionTemps);
    SDL_Flip(ecran);

    TTF_CloseFont(police);
    TTF_Quit();
    return;
}
