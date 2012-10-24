#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generation.h"
#include "affichage.h"
#include "gestion_fichiers.h"
#include "jeu.h"


void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int score){
    size_t i, j;
    for(i=0; i<nbLignes; i++){
        for(j=0; j<nbColonnes; j++){
            printf("%c", laby[nbColonnes * i + j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Votre SCORE : %d\n", score);
    printf("Aide: Faites deplacer le symbole 'o' vers la sortie, situee en bas a droite.\n");
    return;
}

int afficher_accueil(){
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
    printf("###  Copyrights R. Kos - ENSICAEN 2012-2013  ###\n");
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

void afficher_menu(){
    int choix_menu = 1;
    char texteInfo[81];
    strcpy(texteInfo, " ");
    FILE* fichier = NULL;
    char nomLabyrinthe[31];
    int boolFichierCharge = 0;
    while(choix_menu != 4){
        do{
            system("CLS");
            if(fichier != NULL){
                    printf("INFO : Le fichier charge actuellement est : %s\n", nomLabyrinthe);
            }
            printf("%s\n", texteInfo);
            memset(&texteInfo[0], 0, sizeof(texteInfo));
            printf("---------------------------------------------------------------------------\n");
            printf("| 1. Creer un labyrinthe\n");
            printf("| 2. Charger un labyrinthe\n");
            printf("| 3. Jouer une partie\n");
            printf("| 4. Quitter\n");
            printf("---------------------------------------------------------------------------\n");
            printf("Entrez un numero : ");
            scanf("%d", &choix_menu);
            if(choix_menu < 1 || choix_menu > 4){
                strcpy(texteInfo, "INFO : Choix incorrecte !");
            }
        }while(choix_menu < 1 || choix_menu > 4);

        system("CLS");
        if(choix_menu == 1){
            int nbL, nbC;
            char nomLaby[31];
            printf("INFO : Pour creer un labyrinthe, entrez un nombre de lignes et de colonnes.\n");
            printf("INFO : Ils doivent etre tous les deux impaires et superieurs a 5.\n");
            printf("---------------------------------------------------------------------------\n");
            printf("Nom de labyrinthe : ");
            scanf("%s", nomLaby);
            printf("Nombre de lignes : ");
            scanf("%d", &nbL);
            printf("Nombre de colonnes: ");
            scanf("%d", &nbC);
            char laby [nbL][nbC];
            size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);
            creer_labyrinthe(&(laby[0][0]), N, M);

            FILE* fichier = NULL;
            strcat(nomLaby, ".init");
            fichier = fopen(nomLaby, "w+");
            if (fichier != NULL)
            {
                remplir_fichier(fichier, &(laby[0][0]), N, M);
                strcpy(texteInfo, "INFO : Le labyrinthe a ete cree et enregistre sous ");
                strcat(texteInfo, nomLaby);
                fclose(fichier);
            }
            else
            {
                strcpy(texteInfo, "INFO : Impossible d'ouvrir le fichier ");
                strcat(texteInfo, nomLaby);
            }
        }
        else if(choix_menu == 2){
            printf("INFO : Indiquez le nom du fichier a charger.\n");
            printf("---------------------------------------------------------------------------\n");
            printf("Nom du fichier (sans extension) : ");
            scanf("%s", nomLabyrinthe);
            char nomFichier[31];
            strcpy(nomFichier, nomLabyrinthe);
            strcat(nomFichier, ".init");
            fichier = fopen(nomFichier, "r");

            if (fichier != NULL)
            {
                strcpy(texteInfo, "INFO : Le labyrinthe ");
                strcat(texteInfo, nomLabyrinthe);
                strcat(texteInfo, " a ete charge avec succes");
                boolFichierCharge = 1;
                fclose(fichier);
            }
            else
            {
                boolFichierCharge = 0;
                strcpy(texteInfo, "INFO : Impossible d'ouvrir le fichier ");
                strcat(texteInfo, nomLabyrinthe);
                memset(&nomLabyrinthe[0], 0, sizeof(nomLabyrinthe));
            }

        }
        else if(choix_menu == 3){
            printf("Demarrage d'une nouvelle partie...\n");
            if(boolFichierCharge == 1){
                char nomFichier[31];
                strcpy(nomFichier, nomLabyrinthe);
                strcat(nomFichier, ".init");
                fichier = fopen(nomFichier, "r");
                char chaine[TAILLE_LIGNE] = "";
                int nbLig, nbCol;
                fgets(chaine, TAILLE_LIGNE, fichier);
                nbLig = atoi(chaine);
                memset(&chaine[0], 0, sizeof(chaine));
                fgets(chaine, TAILLE_LIGNE, fichier);
                nbCol = atoi(chaine);
                char laby [nbLig][nbCol];
                size_t N = sizeof(laby) / sizeof(laby[0]), M = sizeof(laby[0]) / sizeof(laby[0][0]);
                remplir_labyrinthe(fichier, &(laby[0][0]), N, M);
                jouer(&(laby[0][0]), N, M, nomLabyrinthe);
                fclose(fichier);
            }
            else{
                strcpy(texteInfo, "INFO : Aucun labyrinthe n'a ete charge !");
            }
        }
    }
    return;
}
