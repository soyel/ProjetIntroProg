#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "affichage.h"

void vider_buffer(void){
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

int main(void)
{
    int choix_utilisateur;

    srand(time(NULL));

    choix_utilisateur = afficher_accueil();
    if(choix_utilisateur==0){
        return 0;
    }
    afficher_menu();
    /*char choix_rejouer = 'A';
    do{
        creer_labyrinthe();
        jouer();
        printf("Bravo! Vous avez gagne!\n");
        do{
             printf("\nVoulez-vous rejouer? (O/N) : ");
             fflush(stdin);
             scanf("%c", &choix_rejouer);
        }while(choix_rejouer != 'O' && choix_rejouer != 'N');
    }while(choix_rejouer == 'O');*/

    return 0;
}
