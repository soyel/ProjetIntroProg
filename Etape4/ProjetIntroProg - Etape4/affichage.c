#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <SDL.h>

#include "jeu.h"
#include "affichage.h"

#include "generation.h"
#include "gestion_fichiers.h"


#ifdef __WIN32__
    #include <windows.h>
    #include <conio.h>
    #define dormir(SEC) Sleep(SEC)
    #define clear() system("cls")
#else
    #include "getch_tool.h"
    #define dormir(SEC) sleep(SEC)
    #define clear() system("clear")
#endif

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

/**
 * Affiche le menu principal
 */
 /*
void afficher_menu()
{
    FILE* fichier = NULL;

    int nbLignes;
    int nbColonnes;
    int choix_menu = 1;

    int fichierCharge = 0;

    int continuerAffichage = 1;

    char nomLaby[31];
    char nomFichier[35];

    while(continuerAffichage)
    {
        do
        {
            clear();
            if(fichierCharge)
            {
                printf("[INFO] : Le labyrinthe charge actuellement : %s\n", nomLaby);
            }
            printf("---------------------------------------------------------------------------\n");
            printf("| 1. Creer un labyrinthe\n");
            printf("| 2. Charger un labyrinthe\n");
            if(fichierCharge)
            {
                printf("| 3. Jouer une partie\n");
                printf("| 4. Trouver chemin\n");
                printf("| 5. Quitter\n");
            }
            else
            {
                printf("| 3. Quitter\n");
            }
            printf("---------------------------------------------------------------------------\n");
            printf("Entrez un numero : ");
            scanf("%d", &choix_menu);
        }
        while(choix_menu < 1 || choix_menu > 5);

        clear();

        if(choix_menu == 1)
        {
            memset(&nomFichier[0], 0, sizeof(nomFichier));
            memset(&nomLaby[0], 0, sizeof(nomLaby));
            printf("\t\tCreation d'un nouveau labyrinthe\n\n");
            printf("---Informations du labyrinthe----------------------------------------------\n\n");
            printf("\tNom du labyrinthe : ");
            scanf("%s", nomLaby);
            printf("\tNombre de lignes  : ");
            scanf("%d", &nbLignes);
            printf("\tNombre de colonnes: ");
            scanf("%d", &nbColonnes);

            char laby [nbLignes][nbColonnes];
            size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);
            creer_labyrinthe(&(laby[0][0]), N, M);

            strcat(nomFichier, nomLaby);
            strcat(nomFichier, ".init");

            fichier = fopen(nomFichier, "w+");

            if (fichier != NULL)
            {
                fichierCharge = 1;
                remplir_fichier(fichier, &(laby[0][0]), N, M);

                printf("\n---------------------------------------------------------------------------\n");
                printf("INFO : Le labyrinthe a ete cree avec succes");

                printf("\n\nAppuyez sur n'importe quelle touche pour revenir au menu principal");
                getch();
                fclose(fichier);
            }
            else
            {
                fichierCharge = 0;
                memset(&nomFichier[0], 0, sizeof(nomFichier));
                memset(&nomLaby[0], 0, sizeof(nomLaby));
                printf("\n\tAttention : Impossible de creer le fichier\n");
                printf("\tAppuyez sur n'importe quelle touche pour revenir au menu principal");
                getch();
            }

        }
        else if(choix_menu == 2)
        {
            printf("\t\tChargement d'un labyrinthe a partir d'un fichier\n\n");
            printf("---Repertoire courant------------------------------------------------------\n\n");

            struct dirent *lecture;
            DIR *rep;
            rep = opendir("." );

            char *p_extension = NULL;
            int unFichierAuMoins = 0;

            while ((lecture = readdir(rep)))
            {
                p_extension = strstr(lecture->d_name, ".init");
                if(p_extension != NULL)
                {
                    unFichierAuMoins = 1;
                    printf("\t%s\n", lecture->d_name);
                }
            }
            if(unFichierAuMoins == 0)
            {
                printf("\tAttention : Aucun labyrinthe enregistre dans ce dossier\n");
                printf("\n---------------------------------------------------------------------------\n");
                printf("Appuyez sur n'importe quelle touche pour revenir au menu principal\n");
                getch();
            }
            else
            {
                memset(&nomFichier[0], 0, sizeof(nomFichier));
                memset(&nomLaby[0], 0, sizeof(nomLaby));
                printf("\n---------------------------------------------------------------------------\n");
                printf("Nom du fichier (sans extension) : ");
                scanf("%s", nomLaby);

                strcpy(nomFichier, nomLaby);
                strcat(nomFichier, ".init");

                fichierCharge = 1;

                fichier = fopen(nomFichier, "r");

                if (fichier != NULL)
                {
                    printf("---------------------------------------------------------------------------\n");
                    printf("INFO : Le labyrinthe a ete charge avec succes");

                    printf("\n\nAppuyez sur n'importe quelle touche pour revenir au menu principal");
                    getch();
                }
                else
                {
                    fichierCharge = 0;
                    memset(&nomFichier[0], 0, sizeof(nomFichier));
                    memset(&nomLaby[0], 0, sizeof(nomLaby));
                    printf("\n\tAttention : Impossible d'ouvrir le fichier\n");
                    printf("\tAppuyez sur n'importe quelle touche pour revenir au menu principal\n");
                    getch();
                }
                fclose(fichier);
            }
            closedir(rep);
        }
        else if(choix_menu == 3)
        {
            if(fichierCharge)
            {
                char choixRecommencer;
                char chaine[TAILLE_LIGNE];

                int i;

                do
                {
                    fichier = fopen(nomFichier, "r");

                    if (fichier != NULL)
                    {
                        memset(&chaine[0], 0, sizeof(chaine));
                        fgets(chaine, TAILLE_LIGNE, fichier);
                        nbLignes = atoi(chaine);

                        memset(&chaine[0], 0, sizeof(chaine));
                        fgets(chaine, TAILLE_LIGNE, fichier);
                        nbColonnes = atoi(chaine);

                        char laby [nbLignes][nbColonnes];
                        size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);

                        lire_fichier_labyrinthe(fichier, &(laby[0][0]), N, M);

                        printf("\n\tLa partie va demarrer dans quelques instants... ");
                        for(i = 3; i >= 1; i--)
                        {
                            printf("%d  ", i);
                            dormir(1000);
                        }

                        jouer(&(laby[0][0]), N, M, nomLaby);

                        printf("\nVoulez-vous recommencer ? (O/N) : ");
                        fflush(stdin);
                        scanf("%c", &choixRecommencer);

                        fclose(fichier);
                    }
                    else
                    {
                        printf("\n\tAttention : Impossible d'ouvrir le fichier\n");
                        printf("\tAppuyez sur n'importe quelle touche pour revenir au menu principal\n");
                        getch();
                        choixRecommencer = 'N';
                    }
                }
                while(choixRecommencer != 'N');
            }
            else
            {
                continuerAffichage = 0;
            }
        }
        else if(choix_menu == 4)
        {
            if(fichierCharge)
            {
                fichier = fopen(nomFichier, "r");

                if (fichier != NULL)
                {
                    char chaine[TAILLE_LIGNE];

                    memset(&chaine[0], 0, sizeof(chaine));
                    fgets(chaine, TAILLE_LIGNE, fichier);
                    nbLignes = atoi(chaine);

                    memset(&chaine[0], 0, sizeof(chaine));
                    fgets(chaine, TAILLE_LIGNE, fichier);
                    nbColonnes = atoi(chaine);

                    char laby [nbLignes][nbColonnes];
                    size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);

                    lire_fichier_labyrinthe(fichier, &(laby[0][0]), N, M);

                    trouver_chemin_de_sortie(&(laby[0][0]), N, M);

                    printf("\nAppuyez sur n'importe quelle touche pour revenir au menu principal");
                    getch();
                }
                else
                {
                    printf("\n\tAttention : Impossible d'ouvrir le fichier\n");
                    printf("\tAppuyez sur n'importe quelle touche pour revenir au menu principal\n");
                    getch();
                }
            }
        }
        else
        {
            if(fichierCharge != 0)
            {
                continuerAffichage = 0;
            }
        }
    }
    return;
}
*/
