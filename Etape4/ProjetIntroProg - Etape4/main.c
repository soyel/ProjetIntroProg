#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmod.h>

void pause();

int main(int argc, char *argv[])
{

    SDL_Surface *ecran = NULL;
    SDL_Surface *imageDeFond = NULL;
    SDL_Surface *imageDeFond2 = NULL;
    SDL_Surface *boutonJouer = NULL;
    SDL_Surface *boutonQuitter = NULL;
    SDL_Surface *titre = NULL;
    SDL_Surface *texte = NULL;

    SDL_Rect positionFond;
    SDL_Rect positionFond2;

    SDL_Rect positionTexte;

    SDL_Rect positionBoutonJouer;
    SDL_Rect positionBoutonQuitter;

    SDL_Rect positionTitre;

    positionBoutonJouer.x = 260;
    positionBoutonJouer.y = 250;

    positionBoutonQuitter.x = 400;
    positionBoutonQuitter.y = 250;

    int tailleBoutonsX = 127;
    int tailleBoutonsY = 48;

    positionFond.x = 0;
    positionFond.y = 0;

    positionFond2.x = 0;
    positionFond2.y = 0;

    FMOD_SYSTEM *system;
    FMOD_SOUND *start;
    FMOD_SOUND *click;

    FMOD_RESULT resultat;
    FMOD_RESULT resultat2;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);

    resultat = FMOD_System_CreateSound(system, "start.wav", FMOD_CREATESAMPLE, 0, &start);
    resultat2 = FMOD_System_CreateSound(system, "click.mp3", FMOD_CREATESAMPLE, 0, &click);

    SDL_Event event;

    int continuer = 1;

    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL
    TTF_Init();

    TTF_Font *police = NULL;
    police = TTF_OpenFont("times.ttf", 25);
    SDL_Color couleurBlanche = {255, 255, 255} ;

    SDL_WM_SetIcon(SDL_LoadBMP("laby.bmp"), NULL); //icone
    ecran = SDL_SetVideoMode(800, 400, 32, SDL_HWSURFACE); // Ouverture de la fenêtre
    SDL_WM_SetCaption("LA'WARA", NULL); //titre

    imageDeFond = SDL_LoadBMP("bg.bmp");
    SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

    boutonJouer = IMG_Load("boutonJouer.png");
    SDL_BlitSurface(boutonJouer, NULL, ecran, &positionBoutonJouer);

    boutonQuitter = IMG_Load("boutonQuitter.png");
    SDL_BlitSurface(boutonQuitter, NULL, ecran, &positionBoutonQuitter);

    SDL_Flip(ecran);
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, start, 0, NULL);
    while (continuer) /* TANT QUE la variable ne vaut pas 0 */
    {
        SDL_WaitEvent(&event); /* On attend un événement qu'on récupère dans event */
        switch(event.type) /* On teste le type d'événement */
        {
            case SDL_QUIT: /* Si c'est un événement QUITTER */
                continuer = 0; /* On met le booléen à 0, donc la boucle va s'arrêter */
                break;
            case SDL_KEYDOWN: /* Si appui sur une touche */
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
                        continuer = 0;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if(event.button.x > positionBoutonQuitter.x && event.button.x < (positionBoutonQuitter.x + tailleBoutonsX) && event.button.y > positionBoutonQuitter.y && event.button.y < (positionBoutonQuitter.y + tailleBoutonsY) )
                    { //si on click gauche sur 'quitter'
                        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                        continuer = 0;
                    }
                    if(event.button.x > positionBoutonJouer.x && event.button.x < (positionBoutonJouer.x + tailleBoutonsX) && event.button.y > positionBoutonJouer.y && event.button.y < (positionBoutonJouer.y + tailleBoutonsY) )
                    { //si on click gauche sur 'jouer'
                        SDL_FreeSurface(boutonJouer);
                        SDL_FreeSurface(boutonQuitter);
                        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                        imageDeFond2 = IMG_Load("bg2.png");
                        SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

                        titre = IMG_Load("titre.png");
                        positionTitre.x = ecran->w / 2 - titre->w / 2;
                        positionTitre.y = 10;

                        texte = TTF_RenderText_Blended(police, "- Créer un labyrinthe        ", couleurBlanche);
                        positionTexte.x = ecran->w / 2 - texte->w / 2;
                        positionTexte.y = 100;

                        SDL_BlitSurface(titre, NULL, ecran, &positionTitre);

                        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
                        SDL_Flip(ecran);

                        texte = TTF_RenderText_Blended(police, "- Charger un labyrinthe      ", couleurBlanche);
                        positionTexte.x = ecran->w / 2 - texte->w / 2;
                        positionTexte.y = 150;

                        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
                        SDL_Flip(ecran);

                        texte = TTF_RenderText_Blended(police, "- Jouer une partie par défaut", couleurBlanche);
                        positionTexte.x = ecran->w / 2 - texte->w / 2;
                        positionTexte.y = 200;

                        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
                        SDL_Flip(ecran);

                        texte = TTF_RenderText_Blended(police, "- Trouver la solution du laby", couleurBlanche);
                        positionTexte.x = ecran->w / 2 - texte->w / 2;
                        positionTexte.y = 250;

                        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
                        SDL_Flip(ecran);

                        texte = TTF_RenderText_Blended(police, "- Quitter                    ", couleurBlanche);
                        positionTexte.x = ecran->w / 2 - texte->w / 2;
                        positionTexte.y = 300;

                        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
                        SDL_Flip(ecran);
                    }
                }
                break;
        }
    }

    FMOD_Sound_Release(start);
    FMOD_Sound_Release(click);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    SDL_FreeSurface(imageDeFond); /* On libère la surface */

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
