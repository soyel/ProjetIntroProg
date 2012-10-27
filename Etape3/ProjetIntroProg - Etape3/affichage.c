#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>

#include "generation.h"
#include "affichage.h"
#include "gestion_fichiers.h"
#include "jeu.h"

#ifdef __WIN32__
    #define clear() system("cls")
#else
    #define clear() system("clear")
#endif

/**
 * Affiche le labyrinthe laby
 * @param laby le labyrinthe à afficher
 * @param nbLignes le nombre de lignes du labyrinthe
 * @param nbColonnes le nombre de colonnes du labyrinthe
 * @param score le score à afficher
 */
void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int score)
{
    printf("\n--NOUVELLE PARTIE-----------------------------------------------------------\n\n");
    size_t i, j;
    for(i = 0; i < nbLignes; i++)
    {
        printf("\t");
        for(j = 0; j < nbColonnes; j++)
        {
            printf("%c", laby[nbColonnes * i + j]);
        }
        printf("\n");
    }
    printf("\n\  [AIDE]:\n");
    printf("\t'f' pour quitter la partie\n");
    printf("\t'i' pour tracer la solution du labyrinthe (mettra fin a la partie)\n\n");
    printf("--RESULTATS-----------------------------------------------------------------\n\n");
    printf("\tVotre SCORE    : %d\n", score);
    return;
}

/**
 * Affiche le menu d'accueil
 * @return 1 si le joueur décide de jouer, 0 sinon
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
    printf("### (c)Copyrights R.Kos - ENSICAEN 2012-2013 ###\n");
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

/**
 * Affiche le menu principal
 */
void afficher_menu()
{
    FILE* fichier = NULL;

    int nbLignes;
    int nbColonnes;
    int choix_menu = 1;
    int i;

    char nomLaby[31];
    char nomFichier[35];

    while(choix_menu != 5)
    {
        do
        {
            clear();
            printf("---------------------------------------------------------------------------\n");
            printf("| 1. Creer un labyrinthe\n");
            printf("| 2. Charger un labyrinthe\n");
            printf("| 3. Jouer une partie\n");
            printf("| 4. Trouver chemin\n");
            printf("| 5. Quitter\n");
            printf("---------------------------------------------------------------------------\n");
            printf("Entrez un numero : ");
            scanf("%d", &choix_menu);

            if(choix_menu < 1 || choix_menu > 5)
            {
                printf("INFO : Choix incorrecte !\n");
            }
        }
        while(choix_menu < 1 || choix_menu > 5);

        clear();
        memset(&nomFichier[0], 0, sizeof(nomFichier));
        memset(&nomLaby[0], 0, sizeof(nomLaby));

        if(choix_menu == 1)
        {
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
                remplir_fichier(fichier, &(laby[0][0]), N, M);

                printf("\n---------------------------------------------------------------------------\n");
                printf("INFO : Le labyrinthe a ete cree avec succes\n");
                printf("\n\tLa partie va demarrer dans quelques instants... ");
                for(i = 3; i >= 1; i--)
                {
                    printf("%d  ", i);
                    Sleep(1000);
                }
                jouer(&(laby[0][0]), N, M, nomLaby, 0);
                fclose(fichier);

                char chaine[TAILLE_LIGNE];
                char choixRecommencer = 1;

                printf("\nVoulez-vous recommencer ? (O/N) : ");
                fflush(stdin);
                scanf("%c", &choixRecommencer);

                while(choixRecommencer != 'N')
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

                        lire_fichier_labyrinthe(fichier, &(laby[0][0]), N, M);

                        printf("\n\tLa partie va demarrer dans quelques instants... ");
                        for(i = 3; i >= 1; i--)
                        {
                            printf("%d  ", i);
                            Sleep(1000);
                        }

                        jouer(&(laby[0][0]), N, M, nomLaby, 0);

                        printf("\nVoulez-vous recommencer ? (O/N) : ");
                        fflush(stdin);
                        scanf("%c", &choixRecommencer);

                        fclose(fichier);
                    }
                    else
                    {
                        printf("\n\tAttention : Impossible d'ouvrir le fichier\n");
                        Sleep(2500);
                        choixRecommencer = 'N';
                    }
                }
            }
            else
            {
                printf("\n\tAttention : Impossible de creer le fichier\n");
                Sleep(2500);
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
                printf("\n---------------------------------------------------------------------------\n");
                printf("Nom du fichier (sans extension) : ");
                scanf("%s", nomLaby);


                strcpy(nomFichier, nomLaby);
                strcat(nomFichier, ".init");

                char chaine[TAILLE_LIGNE];
                char choixRecommencer = 1;

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
                            Sleep(1000);
                        }

                        jouer(&(laby[0][0]), N, M, nomLaby, 0);

                        printf("\nVoulez-vous recommencer ? (O/N) : ");
                        fflush(stdin);
                        scanf("%c", &choixRecommencer);

                        fclose(fichier);
                    }
                    else
                    {
                        printf("\n\tAttention : Impossible d'ouvrir le fichier\n");
                        Sleep(2500);
                        choixRecommencer = 'N';
                    }
                }
                while(choixRecommencer != 'N');
            }
            closedir(rep);
        }
        else if(choix_menu == 3)
        {
            char choixRecommencer = 1;
            do
            {
                clear();
                char laby [9][23];
                size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);
                creer_labyrinthe(&(laby[0][0]), N, M);
                printf("\n\tLa partie va demarrer dans quelques instants... ");
                for(i = 3; i >= 1; i--)
                {
                    printf("%d  ", i);
                    Sleep(1000);
                }
                jouer(&(laby[0][0]), N, M, "par_defaut", 1);
                printf("\nVoulez-vous recommencer ? (O/N) : ");
                fflush(stdin);
                scanf("%c", &choixRecommencer);
            }while(choixRecommencer != 'N');
        }
        else if(choix_menu == 4)
        {
            char laby [9][23];
            size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);
            creer_labyrinthe(&(laby[0][0]), N, M);
            trouver_chemin_de_sortie(&(laby[0][0]), N, M);
        }
        else{
            return;
        }
    }
    return;
}
