#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmod.h>

typedef struct
{
    char texte[31]; // on autorise un nom de 23 caractère
    int enSaisie;
    int indice;
} Saisie;

typedef struct
{
    SDL_Surface *imageDeFond2;
    SDL_Surface *retourMenu;
    SDL_Surface *labelNomLaby;
    SDL_Surface *labelNbLignes;
    SDL_Surface *labelNbColonnes;
    SDL_Rect positionImageDeFond2;
    SDL_Rect positionRetourMenu;
    SDL_Rect positionLabelNomLaby;
    SDL_Rect positionLabelNbLignes;
    SDL_Rect positionLabelNbColonnes;
} InterfaceCreationLaby;

struct coordonnees
{
    int x;
    int y;
};
struct coordonneesMurs
{
    int x;
    int y;
    char type;
};

typedef struct cellule cellule;
struct cellule
{
    int x;
    int y;
    int identifiant;
    struct cellule *celluleSuivante;
    struct cellule *premiereCellule;
};

typedef cellule* chemin;

void afficher_titre_menu();
chemin initialiserChemin(int id, int x, int y);
void ajouterEnfinDeListe(chemin lesChemins, cellule* leChemin);
void creer_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes);
void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, SDL_Surface *ecran);
void trouver_chemin_de_sortie(char *laby, size_t nbLignes, size_t nbColonnes, SDL_Surface *ecran);
void initSaisie(Saisie *saisie);
void saisirCaractere(Saisie *saisie, char caractere);
void finSaisie(Saisie *saisie);
Saisie recuperer_entree(SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby);
char *recuperer_partie_texte(Saisie saisie);
void effacerCaractere(Saisie *saisie);

int main(int argc, char *argv[])
{

    SDL_Surface *ecran = NULL;
    SDL_Surface *imageDeFond = NULL;
    SDL_Surface *imageDeFond2 = NULL;
    SDL_Surface *boutonJouer = NULL;
    SDL_Surface *boutonQuitter = NULL;
    SDL_Surface *fleche = NULL;
    SDL_Surface *retourMenu = NULL;

    SDL_Rect positionFond;
    SDL_Rect positionFond2;

    SDL_Rect positionBoutonJouer;
    SDL_Rect positionBoutonQuitter;

    SDL_Rect positionFleche;
    SDL_Rect positionRetourMenu;


    positionBoutonJouer.x = 260;
    positionBoutonJouer.y = 250;

    positionBoutonQuitter.x = 400;
    positionBoutonQuitter.y = 250;

    positionRetourMenu.x = 550;
    positionRetourMenu.y = 8;

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
    int continuerAffichageLaby = 1;

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

    retourMenu = IMG_Load("retourMenu.png");

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
                                        if(event.button.x > 150 && event.button.x < 694 && event.button.y > 100 && event.button.y < 140)
                                        {
                                            InterfaceCreationLaby iCreationLaby;
                                            iCreationLaby.imageDeFond2 = imageDeFond2;
                                            iCreationLaby.retourMenu = retourMenu;
                                            iCreationLaby.labelNomLaby = IMG_Load("labelNom.png");
                                            iCreationLaby.labelNbLignes = IMG_Load("labelNbLignes.png");
                                            iCreationLaby.labelNbColonnes = IMG_Load("labelNbColonnes.png");
                                            iCreationLaby.positionImageDeFond2 = positionFond2;
                                            iCreationLaby.positionRetourMenu = positionRetourMenu;

                                            iCreationLaby.positionLabelNomLaby.x = 20;
                                            iCreationLaby.positionLabelNomLaby.y = 120;

                                            iCreationLaby.positionLabelNbLignes.x = 20;
                                            iCreationLaby.positionLabelNbLignes.y = 160;

                                            iCreationLaby.positionLabelNbColonnes.x = 20;
                                            iCreationLaby.positionLabelNbColonnes.y = 200;

                                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                                            SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                                            SDL_BlitSurface(iCreationLaby.retourMenu, NULL, ecran, &iCreationLaby.positionRetourMenu);
                                            SDL_BlitSurface(iCreationLaby.labelNomLaby, NULL, ecran, &iCreationLaby.positionLabelNomLaby);
                                            SDL_BlitSurface(iCreationLaby.labelNbLignes, NULL, ecran, &iCreationLaby.positionLabelNbLignes);
                                            SDL_BlitSurface(iCreationLaby.labelNbColonnes, NULL, ecran, &iCreationLaby.positionLabelNbColonnes);
                                            Saisie maSaisie;
                                            SDL_Flip(ecran);
                                            maSaisie = recuperer_entree(ecran, iCreationLaby);

                                        }
                                        if(event.button.x > 150 && event.button.x < 694 && event.button.y > 260 && event.button.y < 300 )
                                        { //si on click gauche sur 'quitter'
                                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                                            continuer = 0;
                                        }
                                        if(event.button.x > 150 && event.button.x < 694 && event.button.y > 180 && event.button.y < 220)
                                        {
                                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                                            char laby [9][23];
                                            size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);
                                            creer_labyrinthe(&(laby[0][0]), N, M);
                                            SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

                                            SDL_BlitSurface(retourMenu, NULL, ecran, &positionRetourMenu);
                                            afficher_labyrinthe(&(laby[0][0]), N, M, ecran);
                                            //trouver_chemin_de_sortie(&(laby[0][0]), N, M, ecran);
                                            continuerAffichageLaby = 1;
                                            while (continuerAffichageLaby) /* TANT QUE la variable ne vaut pas 0 */
                                            {
                                                SDL_WaitEvent(&event); /* On attend un événement qu'on récupère dans event */
                                                switch(event.type) /* On teste le type d'événement */
                                                {
                                                    case SDL_QUIT: /* Si c'est un événement QUITTER */
                                                        continuer = 0;
                                                        continuerAffichageLaby = 0; /* On met le booléen à 0, donc la boucle va s'arrêter */
                                                        break;
                                                    case SDL_KEYDOWN: /* Si appui sur une touche */
                                                        switch (event.key.keysym.sym)
                                                        {
                                                            case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
                                                                continuerAffichageLaby = 0; /* On met le booléen à 0, donc la boucle va s'arrêter */
                                                                SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);
                                                                afficher_titre_menu(ecran);
                                                                SDL_Flip(ecran);
                                                                break;
                                                        }
                                                        break;
                                                    case SDL_MOUSEBUTTONUP:
                                                        if (event.button.button == SDL_BUTTON_LEFT)
                                                        {
                                                            if(event.button.x > 550 && event.button.x < (550 + retourMenu->w) && event.button.y > 8 && event.button.y < (8 + retourMenu->h))
                                                            {
                                                                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                                                                continuerAffichageLaby = 0; /* On met le booléen à 0, donc la boucle va s'arrêter */
                                                                SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);
                                                                afficher_titre_menu(ecran);
                                                                SDL_Flip(ecran);
                                                                break;
                                                            }
                                                        }
                                                        break;
                                                }
                                            }

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
void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, SDL_Surface *ecran)
{
    SDL_Surface *mur = NULL;
    SDL_Surface *solution = NULL;
    SDL_Surface *tresor = NULL;
    SDL_Surface *joueur = NULL;
    SDL_Rect positionCase;

    positionCase.x = 50;
    positionCase.y = 50;

    mur = IMG_Load("mur.png");
    joueur = IMG_Load("kenny.png");
    solution = IMG_Load("solutionIcone.png");
    tresor = IMG_Load("tresor.png");

    size_t i, j;

    for(i = 0; i < nbLignes; i++)
    {
        for(j = 0; j < nbColonnes; j++)
        {
            if(i == 1 && j == 0)
            {
                SDL_BlitSurface(joueur, NULL, ecran, &positionCase);
            }
            if(laby[nbColonnes * i + j] == '#')
            {
                SDL_BlitSurface(mur, NULL, ecran, &positionCase);
            }
            if(laby[nbColonnes * i + j] == '.')
            {
                SDL_BlitSurface(solution, NULL, ecran, &positionCase);
            }
            /*if(laby[nbColonnes * i + j] == 'b')
            {
                SDL_BlitSurface(tresor, NULL, ecran, &positionCase);
            }*/
            positionCase.x += 30;
        }
        positionCase.x = 50;
        positionCase.y += 30;
    }

    SDL_Flip(ecran);
    return;
}
void trouver_chemin_de_sortie(char *laby, size_t nbLignes, size_t nbColonnes, SDL_Surface *ecran)
{
    int position_Actuelle_X = 1;
    int position_Actuelle_Y = 0;

    int direction_x = 0;
    int direction_y = 1;

    while(position_Actuelle_X != nbLignes - 2 || position_Actuelle_Y != nbColonnes - 1)
    {
        //printf("\n--------------------\n");
       // afficher_labyrinthe(laby, nbLignes, nbColonnes, 0);
        //printf("\n--------------------\n");
        //printf("\nPosition Act X : %d\n", position_Actuelle_X);
        //printf("Position Act Y : %d\n", position_Actuelle_Y);
        //printf("DIRECTION X : %d\n", direction_x);
        //printf("DIRECTION Y : %d\n", direction_y);
        //déplacement à droite
        if(direction_x == 0 && direction_y == 1)
        {
            //printf("ON SE DEPLACE A DROITE\n");
            //si là où on va n'est pas un mur
            if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] != '#')
            {
                //printf("IL N Y A PAS DE MUR DEVANT\n");
                //si là où va est -, on y met +
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '.')
                {
                    //printf("ON EST DEJA PASSE ICI\n");
                    laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] = '+';
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        //printf("ON EFFACE LE - DERRIERE NOUS\n");
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }

                //si là où on va est +, on vérifie derriere
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '+')
                {
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        //printf("ON EFFACE LE - DERRIERE NOUS\n");
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                //si là où on est n'est pas +, on y met -
                if(laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] != '+')
                {
                    //printf("ON LAISSE UNE TRACE ICI\n");
                    laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] = '.';
                }


                //si à droite de là où on va n'est pas un mur
                if(laby[nbColonnes * (position_Actuelle_X + 1) + (position_Actuelle_Y + 1)] != '#')
                {
                   //printf("ON TOURNE EN BAS \n");
                    direction_x = 1;
                    direction_y = 0;
                }

                position_Actuelle_X += 0;
                position_Actuelle_Y += 1;
                //printf("ON AVANCE EN %d, %d\n", position_Actuelle_X, position_Actuelle_Y);
            }
            //si c'est un mur
            else
            {
                //printf("ON TOURNE EN HAUT\n");
                direction_x = -1;
                direction_y = 0;
            }
        }
        //direction à gauche
        else if(direction_x == 0 && direction_y == -1)
        {
           //printf("ON SE DEPLACE A GAUCHE\n");
            if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] != '#')
            {
                //printf("IL N Y A PAS DE MUR DEVANT\n");
                //si là où va est -, on y met +
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '.')
                {
                    //printf("ON EST DEJA PASSE ICI\n");
                    laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] = '+';
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        //printf("ON EFFACE LE - DERRIERE NOUS\n");
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }

                //si là où on va est +, on vérifie derriere
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '+')
                {
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        //printf("ON EFFACE LE - DERRIERE NOUS\n");
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                //si là où on est n'est pas +, on y met -
                if(laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] != '+')
                {
                    //printf("ON LAISSE UNE TRACE ICI\n");
                    laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] = '.';
                }

                //si à droite de là où on va n'est pas un mur
                if(laby[nbColonnes * (position_Actuelle_X - 1) + (position_Actuelle_Y - 1)] != '#')
                {
                    //printf("ON TOURNE EN HAUT \n");
                    direction_x = -1;
                    direction_y = 0;
                }

                position_Actuelle_X += 0;
                position_Actuelle_Y += -1;
                //printf("ON AVANCE EN %d, %d\n", position_Actuelle_X, position_Actuelle_Y);
            }
            //si c'est un mur
            else
            {
                //printf("ON TOURNE EN BAS\n");
                direction_x = 1;
                direction_y = 0;
            }
        }
        //direction en haut
        else if(direction_x == -1 && direction_y == 0)
        {
            //printf("ON SE DEPLACE EN HAUT\n");
            //si là où on va n'est pas un mur
            if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] != '#')
            {
                //printf("IL N Y A PAS DE MUR DEVANT\n");
                //si là où va est -, on y met +
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '.')
                {
                    //printf("ON EST DEJA PASSE ICI\n");
                    laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] = '+';
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        //printf("ON EFFACE LE - DERRIERE NOUS\n");
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }

                                //si là où on va est +, on vérifie derriere
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '+')
                {
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        //printf("ON EFFACE LE - DERRIERE NOUS\n");
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }

                //si là où on est n'est pas +, on y met -
                if(laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] != '+')
                {
                    //printf("ON LAISSE UNE TRACE ICI\n");
                    laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] = '.';
                }

                //si à droite de là où on va n'est pas un mur
                if(laby[nbColonnes * (position_Actuelle_X - 1) + (position_Actuelle_Y + 1)] != '#')
                {
                    //printf("ON TOURNE à DROITE \n");
                    direction_x = 0;
                    direction_y = 1;
                }

                position_Actuelle_X += -1;
                position_Actuelle_Y += 0;
                //printf("ON AVANCE EN %d, %d\n", position_Actuelle_X, position_Actuelle_Y);

            }
            //si c'est un mur
            else
            {
                //printf("ON TOURNE A GAUCHE\n");
                direction_x = 0;
                direction_y = -1;
            }
        }
        //direction en bas
        else
        {
            //printf("ON SE DEPLACE EN BAS\n");
            if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] != '#')
            {
                //printf("IL N Y A PAS DE MUR DEVANT\n");
                //si là où va est -, on y met +
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '.')
                {
                    //printf("ON EST DEJA PASSE ICI\n");
                    laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] = '+';
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        //printf("ON EFFACE LE - DERRIERE NOUS\n");
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                                //si là où on va est +, on vérifie derriere
                if(laby[nbColonnes * (position_Actuelle_X + direction_x) + (position_Actuelle_Y + direction_y)] == '+')
                {
                    if(laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] == '.')
                    {
                        //printf("ON EFFACE LE - DERRIERE NOUS\n");
                        laby[nbColonnes * (position_Actuelle_X - direction_x) + (position_Actuelle_Y - direction_y)] = '+';
                    }
                }
                //si là où on est n'est pas +, on y met -
                if(laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] != '+')
                {
                    //printf("ON LAISSE UNE TRACE ICI\n");
                    laby[nbColonnes * position_Actuelle_X + position_Actuelle_Y] = '.';
                }

                //si à droite de là où on va n'est pas un mur
                if(laby[nbColonnes * (position_Actuelle_X + 1) + (position_Actuelle_Y - 1)] != '#')
                {
                    //printf("ON TOURNE A GAUCHE \n");
                    direction_x = 0;
                    direction_y = -1;
                }

                position_Actuelle_X += 1;
                position_Actuelle_Y += 0;
                //printf("ON AVANCE EN %d, %d\n", position_Actuelle_X, position_Actuelle_Y);
            }
            else
            {
                //printf("ON TOURNE A DROITE\n");
                direction_x = 0;
                direction_y = 1;
            }
        }
    }

    int i;
    int j;

    laby[nbColonnes * 1 + 0] = '.';
    laby[nbColonnes * (nbLignes - 2) + (nbColonnes - 1)] = '.';

    for(i = 0; i < nbLignes; i++)
    {

        for(j = 0; j < nbColonnes; j++)
        {
            if(laby[nbColonnes * i + j] == '+' || laby[nbColonnes * i + j] == 'm' || laby[nbColonnes * i + j] == 'b')
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

            //
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
        }
    }
    afficher_labyrinthe(laby, nbLignes, nbColonnes, ecran);
}

Saisie recuperer_entree(SDL_Surface *ecran, InterfaceCreationLaby iCreationLaby)
{
    SDL_Event eventSaisie;

    Saisie saisie;
    Saisie *p_saisie = &saisie;
    initSaisie(p_saisie);
    int continuerSaisie = 1;
    SDL_Surface *texte;
    SDL_Rect positionTexte;

    TTF_Init();
    positionTexte.x = 375;
    positionTexte.y = 125;
    SDL_Color couleurBlanche = {255, 255, 255};

    TTF_Font *police = NULL;
    police = TTF_OpenFont("times.ttf", 24);

    char caractereAinverser = '-';

    while (continuerSaisie) /* TANT QUE la variable ne vaut pas 0 */
    {
        SDL_WaitEvent(&eventSaisie); /* On attend un événement qu'on récupère dans event */
        switch(eventSaisie.type) /* On teste le type d'événement */
        {
            case SDL_MOUSEBUTTONUP:
                if (eventSaisie.button.button == SDL_BUTTON_LEFT)
                {
                    if(eventSaisie.button.x > 550 && eventSaisie.button.x < (550 + iCreationLaby.retourMenu->w) && eventSaisie.button.y > 8 && eventSaisie.button.y < (8 + iCreationLaby.retourMenu->h))
                    { //si on click gauche sur 'quitter'
                        //FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, click, 0, NULL);
                        SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                        afficher_titre_menu(ecran);
                        SDL_Flip(ecran);
                        continuerSaisie = 0;
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch ( eventSaisie.key.keysym.sym ) // on teste quelle touche a été enfoncée
                {
                    case SDLK_a:
                        caractereAinverser = 'q';
                        saisirCaractere(p_saisie, caractereAinverser);
                        break;
                    case SDLK_w:
                        caractereAinverser = 'z';
                        saisirCaractere(p_saisie, caractereAinverser);
                        break;
                    case SDLK_z:
                        caractereAinverser = 'w';
                        saisirCaractere(p_saisie, caractereAinverser);
                        break;
                    case SDLK_q:
                        caractereAinverser = 'a';
                        saisirCaractere(p_saisie, caractereAinverser);
                        break;
                    case SDLK_SEMICOLON:
                        caractereAinverser = 'm';
                        saisirCaractere(p_saisie, caractereAinverser);
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
                        if (saisie.enSaisie == 1)
                        {
                            saisirCaractere(p_saisie, (char)eventSaisie.key.keysym.sym);
                        }  // si on est en train de saisir du texte
                        break;  // le break de tous les case SDLK_*

                    case SDLK_RETURN:  // pour la fin de la saisie
                        finSaisie(p_saisie);
                        continuerSaisie = 0;
                        break;
                    case SDLK_BACKSPACE:
                        effacerCaractere(p_saisie);

                }
                char *chaineT;
                chaineT = recuperer_partie_texte(saisie);
                texte = TTF_RenderText_Blended(police, chaineT, couleurBlanche);

                SDL_BlitSurface(iCreationLaby.imageDeFond2, NULL, ecran, &iCreationLaby.positionImageDeFond2);
                SDL_BlitSurface(iCreationLaby.retourMenu, NULL, ecran, &iCreationLaby.positionRetourMenu);
                SDL_BlitSurface(iCreationLaby.labelNomLaby, NULL, ecran, &iCreationLaby.positionLabelNomLaby);
                SDL_BlitSurface(iCreationLaby.labelNbLignes, NULL, ecran, &iCreationLaby.positionLabelNbLignes);
                SDL_BlitSurface(iCreationLaby.labelNbColonnes, NULL, ecran, &iCreationLaby.positionLabelNbColonnes);
                SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
                SDL_Flip(ecran);
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
}

void saisirCaractere(Saisie *saisie, char caractere)
{
    if (saisie->indice < 31) // s'il reste encore de la place dans le tableau
    {
        int index = saisie->indice;
        char *texte;
        texte = saisie->texte;
        saisie->texte[saisie->indice] = caractere;
        saisie->indice = saisie->indice + 1;
    }
}

void effacerCaractere(Saisie *saisie)
{
    if(saisie->indice != 0)
    {
        saisie->indice = saisie->indice - 1;
    }
}
void finSaisie(Saisie *saisie)
{
    saisie->texte[saisie->indice] = 0; // le 0 terminal
    saisie->enSaisie = 0; // on ne saisie plus
}

char *recuperer_partie_texte(Saisie saisie)
{
    int tailleSaisie = saisie.indice;
    char *chaineTemporaire = NULL;
    chaineTemporaire = malloc(sizeof(int) * tailleSaisie);

    int i;

    for(i = 0; i < tailleSaisie; i++)
    {
        chaineTemporaire[i] = saisie.texte[i];
    }
    chaineTemporaire[i] = 0;
    return chaineTemporaire;
}
