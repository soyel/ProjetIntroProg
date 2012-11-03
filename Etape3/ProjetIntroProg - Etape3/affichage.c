#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "jeu.h"
#include "affichage.h"
#include "generation.h"
#include "gestion_fichiers.h"
#include "tools.h"

#ifdef __WIN32__
    #include <windows.h>
    #include <conio.h>
    #define dormir(SEC) Sleep(SEC)
    #define clear() system("cls")
#else
	#include <unistd.h>
    #include "getch_tool.h"
    #define dormir(SEC) sleep(SEC/1000)
    #define clear() system("clear")
#endif

/**
 * Affiche le labyrinthe laby
 * @param laby le labyrinthe a afficher
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @param score le score a afficher
 */
void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int score, char *nomLaby, Monstre *tabMonstres, int tailleTabMonstres, int temps)
{
    size_t i;
    size_t j;
    char typeMonstre;

    printf("\n--NOUVELLE PARTIE-----------------------------------------------------------\n\n");
    printf("\t%s\n\n", nomLaby);
    for(i = 0; i < nbLignes; i++)
    {
        printf("\t");
        for(j = 0; j < nbColonnes; j++)
        {
            typeMonstre = rechercher_monstre(i, j, tabMonstres, tailleTabMonstres);
            if(typeMonstre != 'n' && laby[nbColonnes * i + j] != 'o')
            {
                printf("%c", typeMonstre);
            }
            else
            {
                printf("%c", laby[nbColonnes * i + j]);
            }
        }
        printf("\n");
    }
    printf("\n\040\040[AIDE]:\n");
    printf("\t'f' pour quitter la partie\n\n");
    printf("--RESULTATS-----------------------------------------------------------------\n\n");
    printf("\tScore          : %d points\n", score);
    printf("\tTemps          : %d sec\n", temps);

    return;
}

/**
 * Affiche le menu d'accueil
 * @return 1 si le joueur decide de jouer, 0 sinon
 */
int afficher_accueil()
{
    int touche = 'o';

    printf("       _/\n");
    printf("      _/          _/_/_/  _/      _/      _/    _/_/_/  _/_/_/_/    _/_/_/\n");
    printf("     _/        _/    _/  _/      _/      _/  _/    _/  _/        _/    _/\n");
    printf("    _/        _/    _/    _/  _/  _/  _/    _/    _/  _/        _/    _/\n");
    printf("   _/_/_/_/    _/_/_/      _/      _/        _/_/_/  _/          _/_/_/\n");
    printf("\n\n\n");
    printf("                               1. Jouer\n");
    printf("                               2. Quitter\n");
    printf("\n");
    printf("                  (c)Copyrights R.Kos - ENSICAEN 2012-2013 ");


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

/**
 * Affiche le menu principal
 */
void afficher_menu()
{
    FILE *fichier = NULL;

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
        switch(choix_menu)
        {
            case 1: menu_creation(fichier, nomFichier, nomLaby, &fichierCharge);
                    break;

            case 2: menu_chargement(fichier, nomFichier, nomLaby, &fichierCharge);
                    break;

            case 3: menu_jouer(fichier, nomFichier, nomLaby, &fichierCharge, &continuerAffichage);
                    break;

            case 4: menu_afficher_solution(fichier, nomFichier, &fichierCharge);
                    break;

            case 5: if(fichierCharge != 0)
                    {
                        continuerAffichage = 0;
                    }
                    break;
        }
    }
    return;
}

void menu_creation(FILE *fichier, char *nomFichier, char *nomLaby, int *fichierCharge)
{
    int nbLignes;
    int nbColonnes;

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

    if(fichier != NULL)
    {
        *fichierCharge = 1;
        remplir_fichier(fichier, &(laby[0][0]), N, M);

        printf("\n---------------------------------------------------------------------------\n");
        printf("INFO : Le labyrinthe a ete cree avec succes");
        #ifndef __WIN32__
            vider_buffer();
        #endif
        printf("\n\nAppuyez sur n'importe quelle touche pour revenir au menu principal");
        getch();
        fclose(fichier);
    }
    else
    {
        *fichierCharge = 0;
        memset(&nomFichier[0], 0, sizeof(nomFichier));
        memset(&nomLaby[0], 0, sizeof(nomLaby));
        printf("\n\tAttention : Impossible de creer le fichier\n");
        #ifndef __WIN32__
            vider_buffer();
        #endif
        printf("\tAppuyez sur n'importe quelle touche pour revenir au menu principal");
        getch();
    }
    return;
}

void menu_chargement(FILE *fichier, char *nomFichier, char *nomLaby, int *fichierCharge)
{
    struct dirent *lecture;
    DIR *rep;

    char *p_extension = NULL;
    int unFichierAuMoins = 0;

    printf("\t\tChargement d'un labyrinthe a partir d'un fichier\n\n");
    printf("---Repertoire courant------------------------------------------------------\n\n");

    rep = opendir("." );
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
        #ifndef __WIN32__
            vider_buffer();
        #endif
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

        *fichierCharge = 1;

        fichier = fopen(nomFichier, "r");

        if (fichier != NULL)
        {
            printf("---------------------------------------------------------------------------\n");
            printf("INFO : Le labyrinthe a ete charge avec succes");
            #ifndef __WIN32__
                vider_buffer();
            #endif
            printf("\n\nAppuyez sur n'importe quelle touche pour revenir au menu principal");
            getch();
            fclose(fichier);
        }
        else
        {
            *fichierCharge = 0;
            memset(&nomFichier[0], 0, sizeof(nomFichier));
            memset(&nomLaby[0], 0, sizeof(nomLaby));
            printf("\n\tAttention : Impossible d'ouvrir le fichier\n");
            #ifndef __WIN32__
                vider_buffer();
            #endif
            printf("\tAppuyez sur n'importe quelle touche pour revenir au menu principal\n");
            getch();
        }

    }
    closedir(rep);
    return;
}

void menu_jouer(FILE *fichier, char *nomFichier, char *nomLaby, int *fichierCharge, int *continuerAff)
{
    char choixRecommencer;
    char chaine[TAILLE_LIGNE];
    int i;
    int nbLignes;
    int nbColonnes;

    if(*fichierCharge)
    {
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
                #ifndef __WIN32__
                    printf("\n");
                #endif
                for(i = 3; i >= 1; i--)
                {
                    #ifdef __WIN32__
                        printf("%d  ", i);
                    #endif
                    dormir(1000);
                }

                jouer(&(laby[0][0]), N, M, nomLaby);

                printf("\nVoulez-vous recommencer ? (O/N) : ");
                choixRecommencer = getchar();
                fclose(fichier);
            }
            else
            {
                printf("\n\tAttention : Impossible d'ouvrir le fichier\n");
                printf("\tAppuyez sur n'importe quelle touche pour revenir au menu principal\n");
                #ifndef __WIN32__
                    vider_buffer();
                #endif
                getch();
                choixRecommencer = 'N';
            }
        }
        while(choixRecommencer != 'N');
    }
    else
    {
        *continuerAff = 0;
    }
}

void menu_afficher_solution(FILE *fichier, char *nomFichier, int *fichierCharge)
{
    int nbLignes;
    int nbColonnes;

    char chaine[TAILLE_LIGNE];

    if(*fichierCharge)
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

            trouver_chemin_de_sortie(&(laby[0][0]), N, M);

            printf("\nAppuyez sur n'importe quelle touche pour revenir au menu principal");
            #ifndef __WIN32__
                vider_buffer();
            #endif
            getch();
        }
        else
        {
            printf("\n\tAttention : Impossible d'ouvrir le fichier\n");
            #ifndef __WIN32__
                vider_buffer();
            #endif
            printf("\tAppuyez sur n'importe quelle touche pour revenir au menu principal\n");
            getch();
        }
    }
}
