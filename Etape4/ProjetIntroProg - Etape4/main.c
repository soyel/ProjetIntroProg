#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmod.h>

void afficher_titre_menu();

int main(int argc, char *argv[])
{

    SDL_Surface *ecran = NULL;
    SDL_Surface *imageDeFond = NULL;
    SDL_Surface *imageDeFond2 = NULL;
    SDL_Surface *boutonJouer = NULL;
    SDL_Surface *boutonQuitter = NULL;
    SDL_Surface *fleche = NULL;

    SDL_Rect positionFond;
    SDL_Rect positionFond2;

    SDL_Rect positionBoutonJouer;
    SDL_Rect positionBoutonQuitter;

    SDL_Rect positionFleche;

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
    ecran = SDL_SetVideoMode(800, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // Ouverture de la fenêtre
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
                        SDL_FreeSurface(imageDeFond);

                        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                        imageDeFond2 = IMG_Load("bg2.png");
                        SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

                        afficher_titre_menu(ecran);

                        fleche = IMG_Load("fleche.png");
                        positionFleche.x = 50;
                        positionFleche.y = 93;

                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);
                        SDL_Flip(ecran);

                        while(continuer)
                        {
                            SDL_WaitEvent(&event); /* On attend un événement qu'on récupère dans event */
                            switch(event.type) /* On teste le type d'événement */
                            {
                                case SDL_QUIT: /* Si c'est un événement QUITTER */
                                    continuer = 0; /* On met le booléen à 0, donc la boucle va s'arrêter */
                                    break;

                                case SDL_KEYDOWN: /* Si appui sur une touche */
                                    if (event.key.keysym.sym == SDLK_ESCAPE)
                                    {
                                            continuer = 0;
                                            break;
                                    }
                                case SDL_MOUSEMOTION:
                                    if(event.motion.x >= 150 && event.motion.x <= 694 && event.motion.y >= 100 && event.motion.y <= 140)
                                    {
                                        SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

                                        afficher_titre_menu(ecran);
                                        positionFleche.y = 100;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                    if(event.motion.x >= 150 && event.motion.x <= 694 && event.motion.y >= 140 && event.motion.y <= 180)
                                    {
                                        SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

                                        afficher_titre_menu(ecran);
                                        positionFleche.y = 140;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                    if(event.motion.x >= 150 && event.motion.x <= 694 && event.motion.y >= 180 && event.motion.y <= 220)
                                    {
                                        SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

                                        afficher_titre_menu(ecran);
                                        positionFleche.y = 180;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                    if(event.motion.x >= 150 && event.motion.x <= 694 && event.motion.y >= 220 && event.motion.y <= 260)
                                    {
                                        SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

                                        afficher_titre_menu(ecran);
                                        positionFleche.y = 220;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                    if(event.motion.x >= 150 && event.motion.x <= 694 && event.motion.y >= 260 && event.motion.y <= 300)
                                    {
                                        SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

                                        afficher_titre_menu(ecran);
                                        positionFleche.y = 260;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                case SDL_MOUSEBUTTONUP:
                                    if (event.button.button == SDL_BUTTON_LEFT)
                                    {
                                        if(event.button.x > 150 && event.button.x < 694 && event.button.y > 260 && event.button.y < 300 )
                                        { //si on click gauche sur 'quitter'
                                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                                            continuer = 0;
                                        }
                                    }
                            }
                        }
                    }
                }
                break;
        }
    }

    FMOD_Sound_Release(start);
    FMOD_Sound_Release(click);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    SDL_FreeSurface(imageDeFond2); /* On libère la surface */
    SDL_FreeSurface(fleche);

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void afficher_titre_menu(SDL_Surface *ecran)
{
    SDL_Surface *chargerPartie;
    SDL_Surface *creerPartie;
    SDL_Surface *jouerClassement;
    SDL_Surface *trouverSolution;
    SDL_Surface *quitterJeu;
    SDL_Surface *titre;

    SDL_Rect positionCreerPartie;
    SDL_Rect positionChargerPartie;
    SDL_Rect positionJouerClassement;
    SDL_Rect positionTrouverSolution;
    SDL_Rect positionQuitterJeu;
    SDL_Rect positionTitre;

    titre = IMG_Load("titre.png");
    positionTitre.x = ecran->w / 2 - titre->w / 2;
    positionTitre.y = 10;

    SDL_BlitSurface(titre, NULL, ecran, &positionTitre);
    creerPartie = IMG_Load("creerLaby.png");
    positionCreerPartie.x = 150;
    positionCreerPartie.y = 100;

    SDL_BlitSurface(creerPartie, NULL, ecran, &positionCreerPartie);

    chargerPartie = IMG_Load("chargerLaby.png");
    positionChargerPartie.x = 150;
    positionChargerPartie.y = 140;

    SDL_BlitSurface(chargerPartie, NULL, ecran, &positionChargerPartie);

    jouerClassement = IMG_Load("jouerClassement.png");
    positionJouerClassement.x = 150;
    positionJouerClassement.y = 180;

    SDL_BlitSurface(jouerClassement, NULL, ecran, &positionJouerClassement);

    trouverSolution = IMG_Load("afficherSolution.png");
    positionTrouverSolution.x = 150;
    positionTrouverSolution.y = 220;

    SDL_BlitSurface(trouverSolution, NULL, ecran, &positionTrouverSolution);

    quitterJeu = IMG_Load("quitterJeu.png");
    positionQuitterJeu.x = 150;
    positionQuitterJeu.y = 260;

    SDL_BlitSurface(quitterJeu, NULL, ecran, &positionQuitterJeu);

    SDL_FreeSurface(titre);
    SDL_FreeSurface(creerPartie);
    SDL_FreeSurface(chargerPartie);
    SDL_FreeSurface(jouerClassement);
    SDL_FreeSurface(trouverSolution);
    SDL_FreeSurface(quitterJeu);
}
