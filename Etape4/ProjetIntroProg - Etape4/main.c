#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmod.h>
#include <dirent.h>
#include <ctype.h>

#include "gestion_fichiers.h"
#include "jeu.h"
#include "generation.h"
#include "affichage.h"

typedef struct
{
    char nomL[31]; // on autorise un nom de 23 caractère
    char nbLignes[3];
    char nbColonnes[3];
    int enSaisie;
    int indice;
    int valide;
} Saisie;


void afficher_titre_menu(SDL_Surface *ecran, int fichierCharge, char *nomFichier);
void initSaisie(Saisie *saisie);
void saisirCaractere(Saisie *saisie, char caractere, int positionEntree);
void finSaisie(Saisie *saisie, int *positionEntree, SDL_Rect *positionIndex);
Saisie recuperer_entree(SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby, int *entreesV);
char *recuperer_partie_texte(Saisie saisie, int positionEntree);
void effacerCaractere(Saisie *saisie);
Saisie fenetre_chargement_fichier(SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby);
void afficher_liste_fichiers(SDL_Surface *ecran);

int main(int argc, char *argv[])
{

    SDL_Surface *ecran = NULL;
    SDL_Surface *imageDeFond = NULL;
    SDL_Surface *boutonJouer = NULL;
    SDL_Surface *boutonQuitter = NULL;
    SDL_Surface *fleche = NULL;
    SDL_Surface *retourMenu = NULL;

    SDL_Rect positionFond;

    SDL_Rect positionBoutonJouer;
    SDL_Rect positionBoutonQuitter;

    SDL_Rect positionFleche;
    SDL_Rect positionRetourMenu;


    positionBoutonJouer.x = 360;
    positionBoutonJouer.y = 450;

    positionBoutonQuitter.x = 615;
    positionBoutonQuitter.y = 450;



    int tailleBoutonsX = 127;
    int tailleBoutonsY = 48;

    positionFond.x = 0;
    positionFond.y = 0;


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
    int continuerAffichageLaby = 1;
    int continuerAffichageSolution = 1;

    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL
    TTF_Init();

    TTF_Font *police = NULL;
    police = TTF_OpenFont("times.ttf", 25);
    SDL_Color couleurBlanche = {255, 255, 255} ;

    SDL_WM_SetIcon(SDL_LoadBMP("images/laby.bmp"), NULL); //icone
    ecran = SDL_SetVideoMode(1100, 700, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // Ouverture de la fenêtre
    SDL_WM_SetCaption("LA'WARA", NULL); //titre

    imageDeFond = IMG_Load("images/bg.png");
    SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

    boutonJouer = IMG_Load("images/boutonJouer.png");
    SDL_BlitSurface(boutonJouer, NULL, ecran, &positionBoutonJouer);

    boutonQuitter = IMG_Load("images/boutonQuitter.png");
    SDL_BlitSurface(boutonQuitter, NULL, ecran, &positionBoutonQuitter);

    SDL_Flip(ecran);
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, start, 0, NULL);

    int fichierCharge = 0;
    int nbLignes;
    int nbColonnes;
    char nomLaby[31];
    char nomFichier[35];
    FILE* fichier = NULL;

    InterfaceCreationLaby iCreationLaby;
    iCreationLaby.imageDeFond2 = IMG_Load("images/bg2.png");
    iCreationLaby.retourMenu = IMG_Load("images/retourMenu.png");;
    iCreationLaby.labelNomLaby = IMG_Load("images/labelNom.png");
    iCreationLaby.labelNbLignes = IMG_Load("images/labelNbLignes.png");
    iCreationLaby.labelNbColonnes = IMG_Load("images/labelNbColonnes.png");
    iCreationLaby.pointeurEntree = IMG_Load("images/pointeur.png");

    iCreationLaby.positionLabelNomLaby.x = 150;
    iCreationLaby.positionLabelNomLaby.y = 220;

    iCreationLaby.positionLabelNbLignes.x = 150;
    iCreationLaby.positionLabelNbLignes.y = 300;

    iCreationLaby.positionLabelNbColonnes.x = 150;
    iCreationLaby.positionLabelNbColonnes.y = 380;

    iCreationLaby.positionPointeurEntree.x = 500;
    iCreationLaby.positionPointeurEntree.y = 225;

    iCreationLaby.positionRetourMenu.x = 550;
    iCreationLaby.positionRetourMenu.y = 8;
    iCreationLaby.positionImageDeFond2.x = 0;
    iCreationLaby.positionImageDeFond2.y = 0;
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
                        SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);

                        afficher_titre_menu(ecran, fichierCharge, nomFichier);

                        fleche = IMG_Load("images/fleche.png");
                        positionFleche.x = 210;
                        positionFleche.y = 220;

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
                                    /* SOURIS SUR "CREER UN LABYRINTHE" */
                                    if(event.motion.x >= 300 && event.motion.x <= 842 && event.motion.y >= 220 && event.motion.y <= 280)
                                    {
                                        SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);

                                        afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                        positionFleche.y = 220;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                    /* SOURIS SUR "CHARGER UN LABYRINTHE" */
                                    if(event.motion.x >= 300 && event.motion.x <= 842 && event.motion.y >= 280 && event.motion.y <= 340)
                                    {
                                        SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);

                                        afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                        positionFleche.y = 280;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                    /* SOURIS SUR "JOUER LA PARTIE*/
                                    if(event.motion.x >= 300 && event.motion.x <= 842 && event.motion.y >= 340 && event.motion.y <= 400 && fichierCharge)
                                    {
                                        SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);

                                        afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                        positionFleche.y = 340;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                    /* SOURIS SUR "AFFICHER LA SOLUTION" */
                                    if(event.motion.x >= 300 && event.motion.x <= 842 && event.motion.y >= 400 && event.motion.y <= 460 && fichierCharge)
                                    {
                                        SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);

                                        afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                        positionFleche.y = 400;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                    /* SOURIS SUR "QUITTER LE JEU" */
                                    if(event.motion.x >= 300 && event.motion.x <= 842 && event.motion.y >= 460 && event.motion.y <= 520)
                                    {
                                        SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);

                                        afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                        positionFleche.y = 460;
                                        SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

                                        SDL_Flip(ecran);
                                    }
                                case SDL_MOUSEBUTTONUP:
                                    if (event.button.button == SDL_BUTTON_LEFT)
                                    {
                                        /* ************************/
                                        /* SI ON APPUI SUR CREER  */
                                        /* ************************/
                                        if(event.button.x > 300 && event.button.x < 842 && event.button.y > 220 && event.button.y < 280)
                                        {

                                            int entreeValidees = 1;
                                            int *p_entreeV = &entreeValidees;

                                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                                            iCreationLaby.positionPointeurEntree.y = 225;
                                            iCreationLaby.positionLabelNomLaby.y = 220;
                                            SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                                            SDL_BlitSurface(iCreationLaby.retourMenu, NULL, ecran, &iCreationLaby.positionRetourMenu);
                                            SDL_BlitSurface(iCreationLaby.labelNomLaby, NULL, ecran, &iCreationLaby.positionLabelNomLaby);
                                            SDL_BlitSurface(iCreationLaby.labelNbLignes, NULL, ecran, &iCreationLaby.positionLabelNbLignes);
                                            SDL_BlitSurface(iCreationLaby.labelNbColonnes, NULL, ecran, &iCreationLaby.positionLabelNbColonnes);
                                            SDL_BlitSurface(iCreationLaby.pointeurEntree, NULL, ecran, &iCreationLaby.positionPointeurEntree);
                                            Saisie maSaisie;
                                            SDL_Flip(ecran);
                                            maSaisie = recuperer_entree(ecran, iCreationLaby, p_entreeV);
                                            SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                                            if(maSaisie.valide)
                                            {
                                                nbLignes = atoi(maSaisie.nbLignes);
                                                nbColonnes = atoi(maSaisie.nbColonnes);
                                                char laby [nbLignes][nbColonnes];
                                                size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);
                                                creer_labyrinthe(&(laby[0][0]), N, M);
                                                memset(&nomFichier[0], 0, sizeof(nomFichier));
                                                memset(&nomLaby[0], 0, sizeof(nomLaby));
                                                strcpy(nomLaby, maSaisie.nomL);
                                                strcat(nomFichier, nomLaby);
                                                strcat(nomFichier, ".init");
                                                fichier = fopen(nomFichier, "w+");

                                                if (fichier != NULL)
                                                {
                                                    fichierCharge = 1;
                                                    remplir_fichier(fichier, &(laby[0][0]), N, M);
                                                    fclose(fichier);
                                                }
                                                else
                                                {
                                                    fichierCharge = 0;
                                                    memset(&nomFichier[0], 0, sizeof(nomFichier));
                                                    memset(&nomLaby[0], 0, sizeof(nomLaby));
                                                }
                                            }
                                            afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                            SDL_Flip(ecran);

                                        }
                                         /* ************************/
                                        /* SI ON APPUI SUR CHARGER */
                                        /* ************************/
                                        if(event.button.x > 300 && event.button.x < 842 && event.button.y > 280 && event.button.y < 340)
                                        {
                                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                                            SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                                            SDL_BlitSurface(iCreationLaby.retourMenu, NULL, ecran, &iCreationLaby.positionRetourMenu);
                                            iCreationLaby.positionLabelNomLaby.y = 615;
                                            iCreationLaby.positionPointeurEntree.y = 620;
                                            SDL_BlitSurface(iCreationLaby.labelNomLaby, NULL, ecran, &iCreationLaby.positionLabelNomLaby);
                                            SDL_BlitSurface(iCreationLaby.pointeurEntree, NULL, ecran, &iCreationLaby.positionPointeurEntree);
                                            afficher_liste_fichiers(ecran);
                                            SDL_Flip(ecran);
                                            Saisie maSaisie;
                                            maSaisie = fenetre_chargement_fichier(ecran, iCreationLaby);
                                            SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);

                                            strcpy(nomLaby, maSaisie.nomL);
                                            strcpy(nomFichier, nomLaby);
                                            strcat(nomFichier, ".init");

                                            fichierCharge = 1;

                                            fichier = fopen(nomFichier, "r");

                                            if (fichier == NULL)
                                            {
                                                fichierCharge = 0;
                                                memset(&nomFichier[0], 0, sizeof(nomFichier));
                                                memset(&nomLaby[0], 0, sizeof(nomLaby));
                                            }
                                            fclose(fichier);
                                            afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                            SDL_Flip(ecran);


                                        }
                                        /* ************************/
                                        /* SI ON APPUI SUR JOUER  */
                                        /* ************************/
                                        if(event.button.x > 300 && event.button.x < 842 && event.button.y > 340 && event.button.y < 400)
                                        {
                                            char chaine[10];
                                            if(fichierCharge)
                                            {
                                                fichier = fopen(nomFichier, "r");

                                                if (fichier != NULL)
                                                {
                                                    memset(&chaine[0], 0, sizeof(chaine));
                                                    fgets(chaine, 10, fichier);
                                                    nbLignes = atoi(chaine);

                                                    memset(&chaine[0], 0, sizeof(chaine));
                                                    fgets(chaine, 10, fichier);
                                                    nbColonnes = atoi(chaine);

                                                    char laby [nbLignes][nbColonnes];
                                                    size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);

                                                    lire_fichier_labyrinthe(fichier, &(laby[0][0]), N, M);

                                                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);

                                                    jouer(&(laby[0][0]), N, M, nomLaby, ecran, iCreationLaby);
                                                    SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                                                    afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                                    SDL_Flip(ecran);
                                                    fclose(fichier);
                                                }
                                                else
                                                {
                                                    fichierCharge = 0;
                                                    memset(&nomFichier[0], 0, sizeof(nomFichier));
                                                    memset(&nomLaby[0], 0, sizeof(nomLaby));
                                                }
                                            }
                                        }
                                        /* *************************/
                                        /* SI ON APPUIE SUR AFF SOL*/
                                        /* *************************/
                                        if(event.button.x > 300 && event.button.x < 842 && event.button.y > 400 && event.button.y < 460 )
                                        {
                                            if(fichierCharge)
                                            {
                                                char chaine[10];
                                                fichier = fopen(nomFichier, "r");

                                                if (fichier != NULL)
                                                {
                                                    memset(&chaine[0], 0, sizeof(chaine));
                                                    fgets(chaine, 10, fichier);
                                                    nbLignes = atoi(chaine);

                                                    memset(&chaine[0], 0, sizeof(chaine));
                                                    fgets(chaine, 10, fichier);
                                                    nbColonnes = atoi(chaine);

                                                    char laby [nbLignes][nbColonnes];
                                                    size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);

                                                    lire_fichier_labyrinthe(fichier, &(laby[0][0]), N, M);

                                                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);

                                                    trouver_chemin_de_sortie(&(laby[0][0]), N, M, ecran, iCreationLaby);
                                                    continuerAffichageSolution = 1;

                                                    while(continuerAffichageSolution)
                                                    {
                                                        SDL_WaitEvent(&event); /* On attend un événement qu'on récupère dans event */
                                                        switch(event.type) /* On teste le type d'événement */
                                                        {
                                                            case SDL_QUIT: /* Si c'est un événement QUITTER */
                                                                continuerAffichageSolution = 0; /* On met le booléen à 0, donc la boucle va s'arrêter */
                                                                break;

                                                            case SDL_MOUSEBUTTONUP:
                                                                if (event.button.button == SDL_BUTTON_LEFT)
                                                                {
                                                                    if(event.button.x > 550 && event.button.x < (550 + iCreationLaby.retourMenu->w) && event.button.y > 8 && event.button.y < (8 + iCreationLaby.retourMenu->h))
                                                                    {
                                                                        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                                                                        continuerAffichageSolution = 0;
                                                                    }
                                                                }
                                                                break;
                                                            case SDL_KEYDOWN:
                                                                switch (event.key.keysym.sym ) // on teste quelle touche a été enfoncée
                                                                {
                                                                    case SDLK_ESCAPE:
                                                                        continuerAffichageSolution = 0;
                                                                        break;
                                                                }
                                                        }
                                                    }

                                                    SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                                                    afficher_titre_menu(ecran, fichierCharge, nomFichier);
                                                    SDL_Flip(ecran);
                                                    fclose(fichier);
                                                }
                                                else
                                                {
                                                    fichierCharge = 0;
                                                    memset(&nomFichier[0], 0, sizeof(nomFichier));
                                                    memset(&nomLaby[0], 0, sizeof(nomLaby));
                                                }
                                            }
                                        }
                                        /* *************************/
                                        /* SI ON APPUIE SUR QUITTER*/
                                        /* *************************/
                                        if(event.button.x > 300 && event.button.x < 842 && event.button.y > 460 && event.button.y < 520 )
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

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void afficher_titre_menu(SDL_Surface *ecran, int fichierCharge, char *nomFichier)
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

    titre = IMG_Load("images/titre.png");
    positionTitre.x = ecran->w / 2 - titre->w / 2;
    positionTitre.y = 10;

    SDL_BlitSurface(titre, NULL, ecran, &positionTitre);
    creerPartie = IMG_Load("images/creerLaby.png");
    positionCreerPartie.x = 300;
    positionCreerPartie.y = 220;

    SDL_BlitSurface(creerPartie, NULL, ecran, &positionCreerPartie);

    chargerPartie = IMG_Load("images/chargerLaby.png");
    positionChargerPartie.x = 300;
    positionChargerPartie.y = 280;

    SDL_BlitSurface(chargerPartie, NULL, ecran, &positionChargerPartie);

    if(fichierCharge)
    {
        jouerClassement = IMG_Load("images/jouerPartie.png");
        positionJouerClassement.x = 300;
        positionJouerClassement.y = 340;

        SDL_BlitSurface(jouerClassement, NULL, ecran, &positionJouerClassement);

        trouverSolution = IMG_Load("images/afficherSolution.png");
        positionTrouverSolution.x = 300;
        positionTrouverSolution.y = 400;

        SDL_BlitSurface(trouverSolution, NULL, ecran, &positionTrouverSolution);

        SDL_FreeSurface(jouerClassement);
        SDL_FreeSurface(trouverSolution);
    }

    quitterJeu = IMG_Load("images/quitterJeu.png");
    positionQuitterJeu.x = 300;
    positionQuitterJeu.y = 460;

    SDL_BlitSurface(quitterJeu, NULL, ecran, &positionQuitterJeu);

    SDL_FreeSurface(titre);
    SDL_FreeSurface(creerPartie);
    SDL_FreeSurface(chargerPartie);
    SDL_FreeSurface(quitterJeu);
}

void afficher_liste_fichiers(SDL_Surface *ecran)
{
    struct dirent *lecture;
    DIR *rep;
    rep = opendir("." );

    char *p_extension = NULL;
    int unFichierAuMoins = 0;
    SDL_Surface *elementFichier;
    SDL_Surface *contourListe;
    SDL_Rect positionContourListe;
    SDL_Rect positionElementFichier;
    positionContourListe.x = 0;
    positionContourListe.y = 150;
    positionElementFichier.x = 100;
    positionElementFichier.y = 230;
    TTF_Init();
    SDL_Color couleurBlanche = {255, 255, 255};
    TTF_Font *police = NULL;
    police = TTF_OpenFont("times.ttf", 20);
    contourListe = IMG_Load("images/affichageListe.png");
    SDL_BlitSurface(contourListe, NULL, ecran, &positionContourListe);
    while ((lecture = readdir(rep)))
    {
        p_extension = strstr(lecture->d_name, ".init");
        if(p_extension != NULL)
        {
            unFichierAuMoins = 1;
            elementFichier = TTF_RenderText_Blended(police, lecture->d_name, couleurBlanche);
            SDL_BlitSurface(elementFichier, NULL, ecran, &positionElementFichier);
            positionElementFichier.y += 20;
        }
    }
    TTF_CloseFont(police);
    TTF_Quit();
}
Saisie fenetre_chargement_fichier(SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby)
{
    SDL_Event eventSaisie;

    Saisie saisie;
    Saisie *p_saisie = &saisie;
    initSaisie(p_saisie);

    SDL_Surface *nomLaby;

    SDL_Rect positionNomLaby;
    positionNomLaby.x = 535;
    positionNomLaby.y = 620;

    TTF_Init();
    SDL_Color couleurBlanche = {255, 255, 255};
    TTF_Font *police = NULL;
    police = TTF_OpenFont("times.ttf", 24);
    nomLaby = TTF_RenderText_Blended(police, "", couleurBlanche);

    int continuerSaisie = 1;
    int entierVide = 1;
    int *p_vide = &entierVide;
    char caractereAinverser;
    char *chaineT;
    while(continuerSaisie)
    {
        SDL_WaitEvent(&eventSaisie);
        switch(eventSaisie.type)
        {
            case SDL_MOUSEBUTTONUP:
                if (eventSaisie.button.button == SDL_BUTTON_LEFT)
                {
                    if(eventSaisie.button.x > 550 && eventSaisie.button.x < (550 + iCreationLaby.retourMenu->w) && eventSaisie.button.y > 8 && eventSaisie.button.y < (8 + iCreationLaby.retourMenu->h))
                    {
                        //FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                        continuerSaisie = 0;
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch (eventSaisie.key.keysym.sym ) // on teste quelle touche a été enfoncée
                {
                    case SDLK_ESCAPE:
                        continuerSaisie = 0;
                        break;
                    case SDLK_a:
                        caractereAinverser = 'q';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), 1);
                        chaineT = recuperer_partie_texte(saisie, 1);
                        nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_w:
                        caractereAinverser = 'z';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), 1);
                        chaineT = recuperer_partie_texte(saisie, 1);
                        nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_z:
                        caractereAinverser = 'w';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), 1);
                        chaineT = recuperer_partie_texte(saisie, 1);
                        nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_q:
                        caractereAinverser = 'a';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), 1);
                        chaineT = recuperer_partie_texte(saisie, 1);
                        nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_SEMICOLON:
                        caractereAinverser = 'm';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), 1);
                        chaineT = recuperer_partie_texte(saisie, 1);
                        nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_b:
                    case SDLK_c:
                    case SDLK_d:
                    case SDLK_e:
                    case SDLK_f:
                    case SDLK_g:
                    case SDLK_h:
                    case SDLK_i:
                    case SDLK_j:
                    case SDLK_k:
                    case SDLK_l:
                    case SDLK_m:
                    case SDLK_n:
                    case SDLK_o:
                    case SDLK_p:
                    case SDLK_r:
                    case SDLK_s:
                    case SDLK_t:
                    case SDLK_u:
                    case SDLK_v:
                    case SDLK_x:
                    case SDLK_y:
                    case SDLK_0:
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                    case SDLK_5:
                    case SDLK_6:
                    case SDLK_7:
                    case SDLK_8:
                    case SDLK_9:
                        if (saisie.enSaisie == 1)
                        {
                            saisirCaractere(p_saisie, toupper((char)eventSaisie.key.keysym.sym), 1);
                            chaineT = recuperer_partie_texte(saisie, 1);
                            nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        }
                        break;

                    case SDLK_RETURN:
                        finSaisie(p_saisie, p_vide, &(iCreationLaby.positionPointeurEntree));
                        if(*p_vide > 1)
                        {
                            saisie.enSaisie = 0;
                            continuerSaisie = 0;
                            saisie.valide = 1;
                        }
                        break;
                    case SDLK_BACKSPACE:
                        effacerCaractere(p_saisie);
                        chaineT = recuperer_partie_texte(saisie, 1);
                        nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);

                }
                if (*p_vide < 2)
                {
                    SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                    afficher_liste_fichiers(ecran);
                    SDL_BlitSurface(iCreationLaby.retourMenu, NULL, ecran, &iCreationLaby.positionRetourMenu);
                    SDL_BlitSurface(iCreationLaby.labelNomLaby, NULL, ecran, &iCreationLaby.positionLabelNomLaby);
                    SDL_BlitSurface(iCreationLaby.pointeurEntree, NULL, ecran, &iCreationLaby.positionPointeurEntree);
                    SDL_BlitSurface(nomLaby, NULL, ecran, &positionNomLaby);
                    SDL_Flip(ecran);
                }
                break;
        }
    }
    TTF_CloseFont(police);
    TTF_Quit();
    return saisie;
}
Saisie recuperer_entree(SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby, int *entreesV)
{
    SDL_Event eventSaisie;

    Saisie saisie;
    Saisie *p_saisie = &saisie;
    initSaisie(p_saisie);
    int continuerSaisie = 1;
    int positionEntree = 1;
    SDL_Surface *nomLaby;
    SDL_Surface *nbL;
    SDL_Surface *nbC;
    SDL_Rect positionNomLaby;
    positionNomLaby.x = 535;
    positionNomLaby.y = 225;

    SDL_Rect positionNbL;
    positionNbL.x = 535;
    positionNbL.y = 305;

    SDL_Rect positionNbC;
    positionNbC.x = 535;
    positionNbC.y = 385;

    TTF_Init();
    SDL_Color couleurBlanche = {255, 255, 255};

    TTF_Font *police = NULL;
    police = TTF_OpenFont("times.ttf", 24);
    nomLaby = TTF_RenderText_Blended(police, "", couleurBlanche);
    nbL = TTF_RenderText_Blended(police, "", couleurBlanche);
    nbC = TTF_RenderText_Blended(police, "", couleurBlanche);
    char *chaineT;
    char caractereAinverser = '-';

    while (continuerSaisie && *entreesV < 4) /* TANT QUE la variable ne vaut pas 0 */
    {
        SDL_WaitEvent(&eventSaisie); /* On attend un événement qu'on récupère dans event */
        switch(eventSaisie.type) /* On teste le type d'événement */
        {
            case SDL_MOUSEBUTTONUP:
                if (eventSaisie.button.button == SDL_BUTTON_LEFT)
                {
                    if(eventSaisie.button.x > 550 && eventSaisie.button.x < (550 + iCreationLaby.retourMenu->w) && eventSaisie.button.y > 8 && eventSaisie.button.y < (8 + iCreationLaby.retourMenu->h))
                    {
                        //FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                        continuerSaisie = 0;
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch (eventSaisie.key.keysym.sym ) // on teste quelle touche a été enfoncée
                {
                    case SDLK_ESCAPE:
                        continuerSaisie = 0;
                        break;
                    case SDLK_a:
                        caractereAinverser = 'q';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), *entreesV);
                        chaineT = recuperer_partie_texte(saisie, *entreesV);
                        if(*entreesV == 1) nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 2) nbL = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 3) nbC = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_w:
                        caractereAinverser = 'z';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), *entreesV);
                        chaineT = recuperer_partie_texte(saisie, *entreesV);
                        if(*entreesV == 1) nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 2) nbL = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 3) nbC = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_z:
                        caractereAinverser = 'w';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), *entreesV);
                        chaineT = recuperer_partie_texte(saisie, *entreesV);
                        if(*entreesV == 1) nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 2) nbL = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 3) nbC = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_q:
                        caractereAinverser = 'a';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), *entreesV);
                        chaineT = recuperer_partie_texte(saisie, *entreesV);
                        if(*entreesV == 1) nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 2) nbL = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 3) nbC = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_SEMICOLON:
                        caractereAinverser = 'm';
                        saisirCaractere(p_saisie, toupper(caractereAinverser), *entreesV);
                        chaineT = recuperer_partie_texte(saisie, *entreesV);
                        if(*entreesV == 1) nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 2) nbL = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 3) nbC = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        break;
                    case SDLK_b:
                    case SDLK_c:
                    case SDLK_d:
                    case SDLK_e:
                    case SDLK_f:
                    case SDLK_g:
                    case SDLK_h:
                    case SDLK_i:
                    case SDLK_j:
                    case SDLK_k:
                    case SDLK_l:
                    case SDLK_m:
                    case SDLK_n:
                    case SDLK_o:
                    case SDLK_p:
                    case SDLK_r:
                    case SDLK_s:
                    case SDLK_t:
                    case SDLK_u:
                    case SDLK_v:
                    case SDLK_x:
                    case SDLK_y:
                    case SDLK_0:
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                    case SDLK_5:
                    case SDLK_6:
                    case SDLK_7:
                    case SDLK_8:
                    case SDLK_9:
                        if (saisie.enSaisie == 1)
                        {
                            saisirCaractere(p_saisie, toupper((char)eventSaisie.key.keysym.sym), *entreesV);
                            chaineT = recuperer_partie_texte(saisie, *entreesV);
                            if(*entreesV == 1) nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                            if(*entreesV == 2) nbL = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                            if(*entreesV == 3) nbC = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        }
                        break;

                    case SDLK_RETURN:  // pour la fin de la saisie
                        finSaisie(p_saisie, entreesV, &(iCreationLaby.positionPointeurEntree));
                        if(*entreesV > 3)
                        {
                            saisie.enSaisie = 0; // on ne saisie plus
                            continuerSaisie = 0;
                            saisie.valide = 1;
                        }
                        break;
                    case SDLK_BACKSPACE:
                        effacerCaractere(p_saisie);
                        chaineT = recuperer_partie_texte(saisie, *entreesV);
                        if(*entreesV == 1) nomLaby = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 2) nbL = TTF_RenderText_Blended(police, chaineT, couleurBlanche);
                        if(*entreesV == 3) nbC = TTF_RenderText_Blended(police, chaineT, couleurBlanche);

                }
                if (*entreesV < 4)
                {
                    SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                    SDL_BlitSurface(iCreationLaby.retourMenu, NULL, ecran, &iCreationLaby.positionRetourMenu);
                    SDL_BlitSurface(iCreationLaby.labelNomLaby, NULL, ecran, &iCreationLaby.positionLabelNomLaby);
                    SDL_BlitSurface(iCreationLaby.labelNbLignes, NULL, ecran, &iCreationLaby.positionLabelNbLignes);
                    SDL_BlitSurface(iCreationLaby.labelNbColonnes, NULL, ecran, &iCreationLaby.positionLabelNbColonnes);
                    SDL_BlitSurface(iCreationLaby.pointeurEntree, NULL, ecran, &iCreationLaby.positionPointeurEntree);
                    SDL_BlitSurface(nomLaby, NULL, ecran, &positionNomLaby);
                    SDL_BlitSurface(nbL, NULL, ecran, &positionNbL);
                    SDL_BlitSurface(nbC, NULL, ecran, &positionNbC);
                    SDL_Flip(ecran);
                }
                break; // le break du case SDL_KEYDOWN
        }

    }

    TTF_CloseFont(police);
    TTF_Quit();
    return saisie;
}

void initSaisie(Saisie *saisie)
{
    saisie->enSaisie = 1;  // pour dire que l'on est en train de saisir
    saisie->indice = 0; // on en est au premier caractère.
    saisie->valide = 0;
}

void saisirCaractere(Saisie *saisie, char caractere, int positionEntree)
{
    int index = saisie->indice;
    int chiffre;
    if(positionEntree == 1)
    {
        if(index < 31)
        {
            saisie->nomL[saisie->indice] = caractere;
            saisie->indice = saisie->indice + 1;
        }
    }
    if(positionEntree == 2)
    {
        if(index < 2 && isdigit(caractere) )
        {
            saisie->nbLignes[saisie->indice] = caractere;
            saisie->indice = saisie->indice + 1;
        }
    }
    if(positionEntree == 3)
    {
        if(index < 2 && isdigit(caractere))
        {
            saisie->nbColonnes[saisie->indice] = caractere;
            saisie->indice = saisie->indice + 1;
        }
    }

}

void effacerCaractere(Saisie *saisie)
{
    if(saisie->indice != 0)
    {
        saisie->indice = saisie->indice - 1;
    }
}
void finSaisie(Saisie *saisie, int *positionEntree, SDL_Rect *positionIndex)
{
    int chiffre;
    int positionE = *positionEntree;
    if(saisie->indice != 0)
    {
        if(positionE == 1)
        {

            saisie->nomL[saisie->indice] = 0; // le 0 terminal
            *positionEntree += 1;
            positionIndex->y += 80;
        }
        if(positionE == 2)
        {
            chiffre = atoi(saisie->nbLignes);
            if(chiffre >= 5 && chiffre % 2 != 0)
            {
                saisie->nbLignes[saisie->indice] = 0; // le 0 terminal
                *positionEntree += 1;
                positionIndex->y += 80;
            }
        }
        if(positionE == 3)
        {
            chiffre = atoi(saisie->nbLignes);
            if(chiffre >= 5 && chiffre % 2 != 0)
            {
                saisie->nbLignes[saisie->indice] = 0; // le 0 terminal
                *positionEntree += 1;
                positionIndex->y += 80;
            }
        }
        saisie->indice = 0;
    }
}

char *recuperer_partie_texte(Saisie saisie, int positionEntree)
{
    int tailleSaisie = saisie.indice;
    char *chaineTemporaire = NULL;
    chaineTemporaire = malloc(sizeof(int) * tailleSaisie);

    int i;

    for(i = 0; i < tailleSaisie; i++)
    {
        if(positionEntree == 1) chaineTemporaire[i] = saisie.nomL[i];
        if(positionEntree == 2) chaineTemporaire[i] = saisie.nbLignes[i];
        if(positionEntree == 3) chaineTemporaire[i] = saisie.nbColonnes[i];

    }
    chaineTemporaire[i] = 0;
    return chaineTemporaire;
}
